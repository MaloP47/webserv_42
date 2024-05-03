# include "Config.hpp"
# include "webserv.h"

void errorCGI(std::string str, int tmpfd, int stdout, int stdin)
{
    std::cerr << "Error with " << str << " in executeCGI()" << std::endl;
    return (close(tmpfd), close(stdout), close(stdin), -1);
    return ;
}

//soit prends obj response pour le remplir avec le body soit retourne un obj body 
int executeCGI(HttpResponse *response, char **env) //needs env for execve and the cgi to execute
{
    char buffer[4096];
    int tmpfd = open(".tmpfile", O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (tmpfd == -1)
    {
        std::cerr << "Error with open() in executeCGI()" << std::endl;
        return (-1);
    }
    int stdout = dup(STDOUT_FILENO);
    int stdin = dup(STDIN_FILENO); //maybe not needed
    if (stdout == -1 || stdin == -1)
    {
        std::cerr << "Error with dup() in executeCGI()" << std::endl; // how to treat errors of basic functions (return, exit, exception, status 500)?
        close(tmpfd);
        return (-1);
    }
    int pid = fork();
    if (pid == -1)
        errorCGI("fork()", tmpfd, stdout, stdin);
    else if (pid == 0) 
    {
        //Child process
        if (dup2(tmpfd, STDOUT_FILENO) == -1)
            errorCGI("dup2()", tmpfd, stdout, stdin);
        close(tmpfd);
        if (execve("", ,env) == -1) //add thing to execute
        {
            errorCGI("execve()", tmpfd, stdout, stdin);
            exit(-1); //exit child process only
        }
    }
    else
    {
        waitpid(0, NULL, 0);
        read(tmpfd, buffer, sizeof(buffer)); //could fail
        // respond->_body += buffer;
        close(tmpfd);
        std::cout << "CGI executed!" << std::endl;
    }
    return (0);
}