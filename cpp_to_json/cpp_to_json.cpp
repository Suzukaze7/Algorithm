#include<fstream>
#include<algorithm>

int main() {
    std::ifstream fin("cpp_to_json\\in.txt");
    std::ofstream fout("cpp_to_json\\out.txt");

    fout << "\"prefix\": \"#\",\n";
    fout << "\"body\": [\n";

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