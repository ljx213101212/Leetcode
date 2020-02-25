#include "../pch.h" 
namespace my_util{

/***
 * PLEASE READ THIS, VERY GOOD RESOURCE.
 * https://docs.microsoft.com/en-us/visualstudio/debugger/how-to-use-the-threads-window?view=vs-2019
 */
class SampleOfMultithreadDebugging{

    public:
        int count = 0;
        void doSomeWork(){
            std::cout << "The doSomeWork function is running on another thread." << std::endl;
            int data = count++;
            // Pause for a moment to provide a delay to make
            // threads more apparent.
            std::this_thread::sleep_for(std::chrono::seconds(3));
            std::string str = std::to_string(data);
            std::cout << str<< std::endl;
        }
        void testMain(){
            std::vector<std::thread> threads;
            for (int i = 0; i < 10; ++i) {
                threads.push_back(std::thread(&my_util::SampleOfMultithreadDebugging::doSomeWork,this));
                std::cout << "The Main() thread calls this after starting the new thread" << std::endl;
            }
            for (auto& thread : threads) {
                thread.join();
            }
        }

};

}