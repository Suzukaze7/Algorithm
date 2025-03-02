#include<fstream>
#include<algorithm>

int main() {
    std::ifstream fin("cpp_to_json\\in.txt");
    std::ofstream fout("cpp_to_json\\out.txt");

    fout << R"("prefix": "@",)" << std::endl;
    fout << R"("body": [)" << std::endl;

    std::string line;
    while (std::getline(fin, line)) {
        fout << '\"';
        std::for_each(line.begin(), line.end(), [&](char c) {
            if (c == '"')
                fout << '\\';
            fout << c;
        });
        fout << "\",\n";
    }

    fout << "]";
}