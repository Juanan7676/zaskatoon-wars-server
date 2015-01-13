#include <string>

namespace tasks
{
	
};
class Task
{
public:
	Task(int TaskID);
	int proccess();
	int TaskID;
	std::string metadata;
};
class InvalidTaskIDException
{
private:
	std::string message;
public:
	InvalidTaskIDException(std::string message);
	std::string what(){return this->message;}
};