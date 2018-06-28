#include"interface.hpp"
int main(int argc, char * argv[])
{
    interface in;
    char prop_file[] = "testcase/prop";
    char model_file[] = "testcase/model";
    int result;
    if(argc == 3)
    {
        result = in.checkmodel(argv[1],argv[2]);
    }
    else
    {
        result = in.checkmodel(model_file, prop_file);
    }
    //cout<<result<<endl;
    return result;
}
