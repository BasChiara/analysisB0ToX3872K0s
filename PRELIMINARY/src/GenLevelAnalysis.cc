#include "../include/MCbase_B0toX3872K0s.h"
#include "../include/MCbase_B0toPsi2SK0s.h"
#include "../include/PrepAnalysis.h"
#include "../include/CheckGenLev.h"
#include "../include/CheckGenLevPsi2S.h"

#include <iostream>
#include <fstream>
#include <dirent.h>
#include <sys/stat.h>

#include <TStyle.h>
#include <TCanvas.h>


using namespace std;

TChain* TChainLoader(const std::string& inputFileName) {


	//================ Loading the directory path from file

	ifstream *inputFile = new ifstream(inputFileName);
	if (inputFile != nullptr) 
		std::cout << " ... [INPUT] " << inputFileName << std::endl;
	
	char Buffer[5000];
	char cDirPath[10000];
	while( !(inputFile->eof()) ){
		inputFile->getline(Buffer,500);
		if (!strstr(Buffer,"#") && !(strspn(Buffer," ") == strlen(Buffer))) sscanf(Buffer,"%s",cDirPath);
	
	}
	std::string DirPath = std::string(cDirPath);
	std::cout << " ... Loading file from directory " << DirPath << std::endl;

	int Nfiles = 0; 
	TString tree_path = "";
	const TString treeName = "/Events";

	//================ Creating chain                                                               

	TChain* chain =new TChain("Events");

	// //================ Read the directory
	struct dirent* file = NULL; 
	struct stat file_stats;
	const char* directory;
	DIR* dir_pointer = NULL;

	directory = cDirPath;
	dir_pointer = opendir(directory);//point to the directory

	while((file = readdir (dir_pointer))){
		if(file == NULL){
			std::cout << "ERROR null pointer to file" << std::endl;
			exit(-1);
		}

		if (strcmp(file->d_name, "xNANO_") < 0) continue; // skip "." and ".." and "log"
        std::string file_path = DirPath + std::string(file->d_name);   
        stat(file_path.c_str(), &file_stats); 
        //std::cout << file_stats.st_size/1000. << " Kb" << std::endl;
        if(file_stats.st_size/1000. < 2378 ) continue;
		Nfiles ++;
		//std::cout << file->d_name << std::endl;
		tree_path = DirPath + "/" + file->d_name + treeName; 
		chain->Add(tree_path);
	}

	std::cout << " ... LOADING " << Nfiles << " FILES ..." << std::endl;
	cout<<" ... Total number of events: " << chain->GetEntries()<<std::endl;

	return chain;
}// TChainLoader()



int main (int argc, char* argv[]){

	

	if(argc < 3){
		std::cout << "... Usage ./CheckGenLevel [Indata directory] [tag] [SGN/NORM]"<< std::endl;
		return 1;
	}	

	std::string DirPath = argv[1];
	TString tag = argv[2];
	std::string channel = argv[3];
	if(channel == "SGN") tag = "X3872_"+ tag; 
	else if (channel == "NORM") tag = "Psi2S_" + tag;

	TChain* chain = TChainLoader(DirPath);

	if (channel == "SGN"){
		CheckGenLev* GenAnalyzer = new CheckGenLev(chain, tag);
		GenAnalyzer->Loop();
		delete GenAnalyzer;
	} else if (channel == "NORM"){
		CheckGenLevPsi2S* GenAnalyzerPsi2S = new CheckGenLevPsi2S(chain, tag);
		GenAnalyzerPsi2S->Loop();
		delete GenAnalyzerPsi2S;
	}

	delete chain;

}//main()
