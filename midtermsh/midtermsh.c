#include <stdio.h>
#include <sys/unistd.h>
#include <string>
#include <sys/wait.h>
#include <sys/types.h>
#include <vector>
#include <algorithm>

using namespace std;
bool exec_ = false;
int current_pid;

void handler(int sig) {
	if(exec_) {
		kill(current_pid, SIGINT);
	}
	write(STDOUT_FILENO, "\n", 1);
	exit(0);
}

std::vector<std::string> split(std::string s, char sp_c) {
	std::vector<std::string> ret;
	while(s.size() > 0) {
		auto iter = std::find(s.begin(), s.end(), sp_c);
		if(iter == s.end()) {
			ret.push_back(s);
			return ret;
		} else {
			ret.push_back(s.substr(0, iter - s.begin()));
			s = s.substr(iter - s.begin() + 1);	
		}
	}

	return ret;
}

int execute (const char* com, char* const* args) {
	int pid = fork();
	if(pid > 0) {
		current_pid = pid;
		exec_ = true;
		wait(&pid);
		exec_ = false;
		return 0;
	} else {
		execvp(com, args);
		return 1;
	}
}

int main() {
	int n = 4096;
	char buf[n];

	signal(SIGINT, handler);

	while(true) {
		write(STDOUT_FILENO, "$ ", 2);
		int size;
		std::string res;
		while(true) {
			size = read(STDIN_FILENO, buf, n);
			
			std::string s(buf, size);
			res = res + s;
			if(buf[size-1] == '\n')
                                break; 
		}

		res = res.substr(0, res.size() - 1);
		
		std::vector<std::string> commands = split(res, '|');			
		for(int i = 0; i < commands.size(); i++) {
			std::vector<std::string> com_args = split(commands[i], ' ');
			std::vector<const char*> args;

			for(int i = 0; i < com_args.size(); i++) {
				if(com_args[i].size() != 0) {
					args.push_back(com_args[i].c_str());
				}
			}
			
		        if(execute(args[0], (char* const*) &args[0])) {
				return 0;
	                }
			
		}
		
	}
	
	return 0;
}
