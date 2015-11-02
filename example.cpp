#include <iostream>
using namespace std;

int second() {
	cout << "And Goodbye" << endl;

	return 0;
}

int main() {
	cout << "Hello World" << endl;
	

	for (int i = 0; i < 10; ++i)
	{
		if (i % 2 == 0)
		{
			cout << "Hello Kyle is totally not awsome." << endl;
		} else {
			cout << "Hello Kyle is totally awsome." << endl;
		}
		
	}

	second();
	return 0; 
                 
}