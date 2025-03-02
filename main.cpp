#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include<fstream>

using namespace std;

//struct with dataset info
struct dataset {
    vector <float> y;
    vector <float> x;
    float N=0;
    float mean_x=0;
    float mean_y=0;
    float stddev_x=0;
    float stddev_y=0;
    //qui di seguito invece metto solo le incertezze che diamo per 
    //buone per motivazioni varie
    float s_x=0; //MESSO ORA PER CASO FORZA, DA CAMBIARE EVENTUALLY
    float s_y=8.167;
};

//struct with regression results (regression of this kind y = a + bx)
struct result {
    float delta_approx=0;
    float delta=0;
    float b_approx=0;
    float a_approx=0;
    float s_b_approx=0;
    float s_a_approx=0;
    float a=0;
    float s_a=0;
    float b=0;
    float s_b=0;
    float post_er=0;
};

void ifilef(dataset&, string);
void meanf(dataset&);
void stddevf(dataset&);
void calcf_approx(dataset&, result&);
void calcf(dataset&, result&);
void post_erf(dataset&, result&);
void ofilef(dataset&, result&, string);

int main() {

    dataset data;
    dataset& datar = data;

    result results;
    result& resultsr = results;

    cout << "numero file input" << endl;
    int i=0;
    cin >> i;
    string filename = "k_hooke_" + to_string(i) + ".txt";


    ifilef(datar, filename);

//queste due funzioni di seguito servono per dare una prima stima 
//di b da usare nelle formule per trovare quello vero
    calcf_approx(datar, resultsr);

//calcolo parametri interpolazione
    calcf(datar, resultsr);

//calcolo errore a posteriori
    post_erf(datar, resultsr);

    ofilef(datar, resultsr, filename);

    return 0;
}

void ifilef(dataset& datar, string filename) {
    ifstream ifile(filename);
    if (!ifile.is_open()) {
        cout << "error code 1";
        exit(1);
    }

    //le seguenti righe sono relative a come leggere i dati 
    //in base alla formattazione
    cout << "se date, di scrivano incertezze date: (se no scrivere 0)" << endl;
    cout << "s_x = ";
    cin >> datar.s_x;
    //cout << "s_y = ";                  TOLTA PER CASO SPECIFICO, DA RIMETTERE
    //cin >> datar.s_y;

    string line;
    getline(ifile, line);
    getline(ifile, line);

    float a=0, b=0;
    while(ifile >> a >> b) {
        datar.x.push_back(b); 
        //MESSO ORA 4 PER CASO SPECIFICO FORZA PROPORZIONALE ALLA LEVA
        datar.y.push_back(a*4);
    }

    if(datar.x.size() != datar.y.size()) {
        cout << "error code 2" << endl;
        exit(1);
    }
    datar.N= datar.x.size();
}

void calcf_approx(dataset& datar, result& resultsr){

    float sumqx=0;
    float sumx=0;
    float sumy=0;
    float sumxy=0;

    for(int i=0;i<datar.N;i++) {
        sumx+=datar.x[i];
        sumy+=datar.y[i];
        sumxy+=datar.x[i]*datar.y[i];
        sumqx+=datar.x[i]*datar.x[i];
    }

    resultsr.delta_approx=datar.N*sumqx-sumx*sumx;
    resultsr.a_approx = 1/resultsr.delta_approx*(sumqx*sumy - sumx*sumxy);
    resultsr.b_approx = 1/resultsr.delta_approx*(datar.N*sumxy - sumx*sumy);
    resultsr.s_a_approx = datar.s_y*sqrt(sumqx/resultsr.delta_approx);
    resultsr.s_b_approx = datar.s_y*sqrt(datar.N/resultsr.delta_approx);
}

void calcf(dataset& datar, result& resultsr) {
    float s=0;
    float S=0;
    float sumx=0;
    float sumy=0;
    float sumxy=0;
    float qsumx=0;

    s = sqrt(datar.s_y*datar.s_y + resultsr.b_approx*
        resultsr.b_approx*datar.s_x*datar.s_x);
    S = 1/(s*s);
    for(auto x:datar.x) sumx += x;
    for(auto y: datar.y) sumy += y;
    for(int i = 0; i < datar.N; i++) sumxy += datar.x[i]*datar.y[i];
    for(auto x: datar.x) qsumx += x*x;

    resultsr.delta = datar.N*S*S*qsumx - S*S*sumx*sumx;
    resultsr.a = 1/resultsr.delta * (S*qsumx*S*sumy - S*S*sumx*sumxy);
    resultsr.b = 1/resultsr.delta * (datar.N*S*S*sumxy - S*S*sumx*sumy);

    resultsr.s_a = sqrt(1/resultsr.delta * S*qsumx);
    resultsr.s_b = sqrt(1/resultsr.delta * S*datar.N);
}

void post_erf(dataset& datar, result& resultsr) {
    float sum=0;
    for(int i=0; i<datar.N; i++) sum += pow(datar.y.at(i)-(resultsr.a + resultsr.b*datar.x[i]), 2);
    resultsr.post_er = sqrt(sum/(datar.N-2));
}

void ofilef(dataset & datar, result & resultsr, string filename) {
    ofstream ofile("results_linear_regression.txt", ios::app);
    ofile << endl << "regression results of y = a + bx for " << filename << endl;
    ofile << "approximation" << endl;
    ofile << resultsr.a_approx << "\t" << resultsr.s_a_approx << endl;
    ofile << resultsr.b_approx << "\t" << resultsr.s_b_approx << endl;
    ofile << "right" << endl;
    ofile << resultsr.a << "\t" << resultsr.s_a << endl;
    ofile << resultsr.b << "\t" << resultsr.s_b << endl;
    ofile << "post error =      " << resultsr.post_er << endl;

}



/*
error codes:
-1 : errore nell'apertura del file in input;
-2 : la dimensione dei dati sulle x e sulle y non coincidono, 
probabile errore di lettura dell'ifile
*/
\end{minted}

\subsection{Codice in C++ per il test del \texorpdfstring{$\chi^2$}{chi2} }

\begin{minted}{C++}
#include<iostream>
#include<cmath>
#include<vector>
#include<string>
#include<fstream>

using namespace std;

struct data {
    double s_x=0;
    double a=0;
    double b=0;
    vector <double> F;
    vector <double> x;
};

struct info {
    double res_sum=0;
    double cf=0;
    double chi2_ref=0;
};

void chi2f(string&, data&, info&);
void routinef(string&, data&, info&);


int main() {

    string file;
    string& filer = file;

    data dati;
    data& datir = dati;

    info res;
    info& resr = res;

    cout << "alfa = ";
    cin >> resr.cf;

    if(resr.cf==0.1) resr.chi2_ref = 13.362;
    else if(resr.cf==0.05) resr.chi2_ref = 15.507;
    else if(resr.cf==0.025) resr.chi2_ref = 17.535;
    else if(resr.cf==0.01) resr.chi2_ref = 20.090;
    else if(resr.cf==0.005) resr.chi2_ref = 21.955;
    else if(resr.cf==0.90) resr.chi2_ref = 3.490;
    else if(resr.cf==0.95) resr.chi2_ref = 2.733;
    else if(resr.cf==0.975) resr.chi2_ref = 2.180;
    else if(resr.cf==0.99) resr.chi2_ref = 1.646;
    else if(resr.cf==0.995) resr.chi2_ref = 1.344;

    routinef(filer, datir, resr);

    return 0;
}


void chi2f(string& filer, data& datir, info& resr) {
    resr.res_sum=0;
    for(int i=0;i<datir.x.size();i++) resr.res_sum+=pow(
    ((datir.F[i]*4-(datir.a+datir.b*datir.x[i]))/datir.s_x),2);
}

void routinef(string &filer, data& datir, info& resr) {

    string skip_line;

    ofstream ofile("chi2_k_results.txt",ios::app);
    if(!ofile.is_open()) cout << "out file failed";

    ifstream stddevfile("mean_stddev_deltax.txt");
    if(!stddevfile.is_open()) cout << "stddev file failed";
    for(int i=0;i<2;i++) getline(stddevfile,skip_line);

    ifstream parafile("results_linear_regression.txt");
    if(!parafile.is_open()) cout << "parameters file failed";
    for(int i=0;i<53;i++) getline(parafile,skip_line);          
    //METTERE 8 O 53 SKIP IN BASE AD ANDATA O RITORNO

    for(int i=1; i<=5; i++) {
        datir.F.clear();
        datir.x.clear();
// QUI PER MODIFICARE NOME FILE
        filer = "k_hooke_-" + to_string(i) + ".txt";

        ifstream ifile(filer);
        if(!ifile.is_open()) cout << "ifile failed";
        getline(ifile,skip_line);
        getline(ifile,skip_line);
        double b=0, c=0;
        while(ifile >> b >> c) {
            datir.F.push_back(b);
            datir.x.push_back(c);
        }

        double a=0, d=0;

        //INCERTEZZA LEGATA A x DA CAPIRE
        stddevfile >> a >> d;
        datir.s_x=8.167; 
        //PRIMA QUESTA PARTE SERVIVA PER LEGGERE L'INCERTEZZA IN ALLUNGAMENTO

        parafile >> datir.a;
        getline(parafile,skip_line);
        parafile >> datir.b;

        getline(stddevfile,skip_line);
        for(int k=0; k<8; k++) getline(parafile,skip_line);

        ifile.close();

        chi2f(filer, datir, resr);

        ofile << "-" << filer << endl;
        if(resr.chi2_ref>resr.res_sum) ofile << 
        "il test del chi2 e' superato con CL = " << resr.cf << endl;
        if(resr.chi2_ref<resr.res_sum) ofile << 
        "il test del chi2 non e' superato con CL = " << resr.cf << endl;
        ofile << "chi2 sper = " << resr.res_sum << 
        "\tchi2 ref = " << resr.chi2_ref << endl << endl;
    }