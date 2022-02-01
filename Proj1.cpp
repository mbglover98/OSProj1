#include <iostream>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <list>
#include <string>
#include <fstream>

char** parseCommandLine(std::string aCommandLine) {
    std::list<std::string> container;
    std::string hold = "";
    for (auto& ch : aCommandLine) {
        if (ch == ' ' || ch == '<' || ch == '>' || ch == '|') {
            if(hold != "")                                                              //prevents empty strings
                container.push_back(hold);
            hold = "";
        }
        else {
            hold = hold + ch;
        }
    }
    container.push_back(hold);

    char** ret = new char*[container.size()+1];                                           //ret is an array of pointers
    int stop = container.size();
    for (int i = 0; i < stop; i++) {                                       //making cstrings

        char* temp = new char[container.front().size()+1];
        for (int j = 0; j <= container.front().size(); j++) {
            *(temp+j) = container.front()[j];
        }
        *(temp + container.front().size() + 1) = '\0';
        container.pop_front();
        
        *(ret + i) = temp;
        
    }
    char* empty = new char[1];
    *(empty) = '\0';
    *(ret+stop) = empty;
    return ret;
}
/*This function sets up and calls other programs*/
pid_t spawnChild(char **args){
    pid_t ch_pid = fork();
    int count = 0;
    while(*(args+count) !=NULL){
        count++;
    }
    char * argv[count];                                     //Creates the *array[]
    for(int i = 0;i<count;i++){
        argv[i] = const_cast<char*>( *args+i);
    }
    argv[count-1] = NULL;
    if(ch_pid > 0){
        std::cout << "Printed from parent\n";
        ch_pid = wait(NULL);
    }
    else {
        std::cout << "Printed from Child\n";
        std:: cout << execv(*args,argv);
        std::cout << "\n";
    }
    return ch_pid;
}

/*This function decides which command is called and executes it*/
void commandPath(char **a){
    char in = **a;
    int i =1;
    std::string out = "";
    char tmp[100];
    while(in != '\0'){
        out += in;
        
        in = *(*a +i);
        i++;
    }

    if(out == "pwd"){
        printf("%s\n",getcwd(tmp,100));
        return;
    }

    if(out == "cd"){
        i = 1;
        char cFilePath = *(*(a+1));
        std::string filePath = "";
        chdir((*(a+1)));
        return;
    }

    if(out == "exit"){
        std::cout << "Exiting\n";
        exit(0);
        return;
    }

    if(out == "cat"){
        std::ifstream myfile;
        std::string in;
        char input = *(*(a+1));
        i=1;
        while(input != '\0'){
            in += input;
            
            input = *(*(a+1) +i);
            i++;
        }
        myfile.open(in);
        std::cout << myfile.rdbuf();
        myfile.close();
        return;
    }
    else{
        spawnChild(a);
    }
}

int main(){
    std::string input;
    int i = 0;
    char tmp[100];
    
    while(1){                                                       //Infinite loop
        printf("BlakeShell:%s:",getcwd(tmp,100));
        std::getline(std::cin,input);
        char **a = parseCommandLine(input);
        commandPath(a);
        i++;
    }
    return 0;
}