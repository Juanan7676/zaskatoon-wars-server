#include <string>

namespace tasks
{
	
};
class Task
{
public:
	Task(int TaskID);
	Task(std::string metadata);
	void push();
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