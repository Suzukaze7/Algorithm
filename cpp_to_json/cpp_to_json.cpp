#include<fstream>
#include<string>

#define in_file "cpp_to_json\\in.txt"
#define out_file "cpp_to_json\\out.txt"

using namespace std;

int main()
{
    ifstream fin(in_file);
    ofstream fout(out_file);

    fout << "\"prefix\": \"#\",\n";
    fout << "\"body\": [\n";

    string line;
    while (getline(fin, line))
        fout << '\"' << line << "\",\n";

    fout << "]";
}