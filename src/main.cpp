
#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <boost/functional/hash.hpp>

/** For random numbers */
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/mersenne_twister.hpp>
/** For random numbers */

using namespace std;

string charArray;
string charArray1 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
string charArray2 = "abcdefghijklmnopqrstuvwxyz";
string charArray3 = "0123456789";
string charArray4 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
string charArray5 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
string charArray6 = "abcdefghijklmnopqrstuvwxyz0123456789";
string charArray7 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

int GetRandom()
{
	static boost::mt19937 generator(static_cast<boost::uint32_t>(time(NULL)));
	static boost::uniform_real<> uni_dist(0,charArray.size() - 1);
	static boost::variate_generator<boost::mt19937, boost::uniform_real<> > uni(generator, uni_dist);

	return static_cast<int>(uni());
}

void CreateSerials(size_t size, string srlTmpl, string outFileName, string preText, string posText)
{
	// Serials
	char *srlNum = new char[srlTmpl.size() + 1];
	multimap<size_t, string> serials;
	multimap<size_t, string>::iterator serialsIt;

	// Hashes
	size_t srlNumHash = 0;
	boost::hash<std::string> string_hash;

	// Files
	ofstream outFile;
	outFile.open(outFileName.c_str());

	vector<size_t> consts;
	string sqlstr;
	int counterShow = 0;
	bool flag = false;
	size_t cnstNum = -1;

	for (size_t p = 0; p < srlTmpl.size(); ++p)
		if (srlTmpl[p] != '*')
			consts.push_back(p);

	size_t k, c = 0;
	for (k = 0; k < size; ++k) {
		for (size_t i = 0; i < srlTmpl.size(); ++i) {
			for (size_t j = 0; j < consts.size(); ++j)
				if (i == consts[j])
					cnstNum = consts[j];

			if (cnstNum != -1)
				srlNum[i] = srlTmpl[cnstNum];
			else
				srlNum[i] = charArray[GetRandom()];
			cnstNum = -1;
		}
		srlNum[srlTmpl.size()] = '\0';

		srlNumHash = string_hash(srlNum);

		serialsIt = serials.find(srlNumHash);

		if (serials.end() != serialsIt) {
			if (serialsIt->second.compare(srlNum) == 0) {
				cout << "We have found double" << endl;
				flag = true;
			}
		}

		if (flag) {
			flag = false;
			continue;
		}

		serials.insert(pair<size_t, string>(srlNumHash, srlNum));

		sqlstr = preText + string(srlNum) + posText;
		outFile.write(sqlstr.c_str(), sqlstr.size());

		if (((c % 1000) == 0) && (c != 0)) {
			cout << k << " keys has been finished." <<endl;
		}
		c++;
	}

	cout << c << " keys has been finished." << endl;

	delete [] srlNum;
}

int main(int argc, char *argv[])
{
	string tmpl, fileName, pret, post; 
	int serialCount, ca;

	cout << "How many serial : ";
	cin >> serialCount;

	cout << endl << "What is the template (use '*' for keys. For example ****-**** or ****-****-****-****) :";
	cin >> tmpl;

	cout << endl << "What is the out file name :";
	cin >> fileName;

	cout << endl << "If you want to use pre text\n" << 
		"example :\"INSERT INTO `product_serial`(`product_serial`) VALUES('\"\n" << 
		"example :\"INSERT INTO `sales_coupon_code`(`coupon_code`) VALUES('\"\n" << 
		"enter now :";
	cin >> pret;

	cout << endl << "If you want post text :\n" << 
		"example :\"');\"\n" << 
		"enter now :";
	cin >> post;
	post += "\n";

	cout << endl << "What is the char array?" << endl;
	cout << " 1 - 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'" << endl;
	cout << " 2 - 'abcdefghijklmnopqrstuvwxyz'" << endl;
	cout << " 3 - '0123456789'" << endl;
	cout << " 4 - 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz'" << endl;
	cout << " 5 - 'ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789'" << endl;
	cout << " 6 - 'abcdefghijklmnopqrstuvwxyz0123456789'" << endl;
	cout << " 7 - 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789'" << endl;
	cin >> ca;

	switch(ca) {
		case 1:
			charArray = charArray1;
			break;
		case 2:
			charArray = charArray2;
			break;
		case 3:
			charArray = charArray3;
			break;
		case 4:
			charArray = charArray4;
			break;
		case 5:
			charArray = charArray5;
			break;
		case 6:
			charArray = charArray6;
			break;
		case 7:
			charArray = charArray7;
			break;
		default:
			charArray = charArray1;
			break;
	}

	CreateSerials(serialCount, tmpl, fileName, pret, post);

	system("pause");
	return 0;
}

