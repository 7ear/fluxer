#include "Fluxer.h"
#include "KEYS.h"

/* ================== VARIABLES ================== */

int user_choose;
string user_input;
vector<string> user_encryption;
string user_file_name;

const string CHARS =
"0123456789"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz";

string bait;
string encryption_text;

/* ================================================ */

void screen() {
	cout << "\033[5m\033[1m\033[34m";

	cout << R"(
   ______                     Fluxer - is a simple tool
  / __/ /_ ____ _____ ____    for encrypting and decrypting
 / _// / // /\ \ / -_) __/    text data.
/_/ /_/\_,_//_\_\\__/_/   
                  
)";
	cout << "\033[0m";

	cout << "\033[35mChoose a mod:\n [1] Encryption\n [2] Decryption\n\033[0m";

	cin >> user_choose;

	cout << "\033[F";
	cout << "\033[K";
}

void encryption() {
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	cout << "\nEnter the text you want to encrypt: ";

	getline(cin, user_input);
	
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	mt19937 generator(seed);
	uniform_int_distribution<int> distribution(0, CHARS.size() - 1);

	for (int i = 0; i < 3; ++i) {
		bait += CHARS[distribution(generator)];
	}

	ofstream encrypt;

	encrypt.open("encrypted.txt");

	try {
		if (encrypt.is_open()) {
			encrypt << bait;
			for (char c : user_input) {
				encrypt << KEYS.at(c);
			}
		}
		cout << "\033[32m\nSUCCESS!\033[0m\n";
	}
	catch(const exception& ex) {
		cout << "[!] ERROR: invalid characters";
	}

	encrypt.close();
}

void decryption() {

	try {
		cout << "\nEnter file(.txt) name: \n";

		cin >> user_file_name;

		cout << "\033[F";
		cout << "\033[K";
		cout << "\033[F";
		cout << "\033[K";

		cout << "\nText: ";

		ifstream decrypt(user_file_name);

		if (decrypt.is_open()) {
			while (getline(decrypt, encryption_text));
		}

		for (int i = 3; i < encryption_text.length(); i+=16) {
			string temp = encryption_text.substr(i, 16);

			auto el = find_if(
				KEYS.begin(),
				KEYS.end(),
				[temp](const auto& pair) {
					return pair.second == temp;
				}
			);

			if (el != KEYS.end()) {
				cout << el -> first;
			}
		}

		decrypt.close();
	}
	catch (const exception& ex) {
		cout << ex.what();
	}

	cout << "\n";
}

int main() {
	screen();

	if (user_choose == 1) encryption();
	else if (user_choose == 2) decryption();
	else cout << "[!] ERROR: you must input only {1} or {2}";

	return 0;
}