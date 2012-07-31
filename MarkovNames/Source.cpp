
#include "MarkovNames.h"

#include <fstream>
#include <iostream>
using namespace std;

int main(int argc, char * argv[]){

	//string names = "Blahoslava, Blahuse, Bojana, Bojka, Boleslava, Bolena, Bolerka, Bohumira, Bohuslava, Bozidara, Boza, Bozena, Bozka, Bratislava, Bretislava, Bretka, Breticka, Bronislava, Branislava, Brana, Branka, Brona, Bronicka, Bronka, Dobrali, Dobromila, Dobromira, Dobroslava, Drahomira, Draha, Drahuse, Drahuska, Draza, Dusana, Dusa, Sudanka, Dusicka, Duska, Jarka, Kvetoslava, Kveta, Kvetka, Kvetuse, Kvetuska, Libera, Liba, Libenka, Libuse, Libuska, Lidmila, Ludmilla, Ludmila, Lida, Lidka, Liduna, Lidunka, Liduse, Lizuska, Lubomira, Luba, Lubena, Lubina, Lubina, Lubka, Lubuska, Mecislava, Melina, Mecka, Mila, Milena, Milady, Miladena, Milana, Mlada, Mladena, Miladka, Milanka, Milenka ,Milka, Miluse, Miluska, Mlaska, Mladuska, Miloslava, Miroslava, Mira, Mirka, Miruska, Nadezda, Nadezhda, Nadeja, Neda, Nedda, Pribislava, Pribena, Próbka, Pribuska, Radomia, Rada, Radlinka, Radoslava, Rada, Rostislava, Rosta, Rostina, Rostinka, Rostuska, Sobeslava, Sobena, Sobeska, Stanislava, Stana, Stanicka, Stanuska, Svetlana, Svetla, Svetlanka, Svetluse, Svetluska, Veleslava, Vela, Velina, Velinka, Velka, Veluska, Venceslava, Vaclava, Vena, Venka, Venuska, Vera, Vierka, Verka, Veruska, Vladimíra, Vladmira, Vladislava, Ladislava, Valeska, Vlasta, Zbyhneva, Zbyna, Zbysa, Zbyhneka, Zbyhneuska, Zdenka, Zdeslava, Zdislava, Desa, Zdeska, Zwisa, Zdiska, Zelislava, Zitomira, Zitka, Zituse, Zivanka, Zivka, Zivuse, Zivuska, Zlata, Zlatina, Zlatinka, Zlatka, Zlatuje, Zlatuska, Zlatana, Zlatunka, Zoila, Zora, Zorah, Blahoslav, Blahos, Blahosek, Bohdan, Bohumil, Bohumír, Bohuslav, Bojan, Bujanek, Bojek, Boleslav, Bolek Borivoj, Bora, Borik, Borek, Borzivoi, Bozidar, Bratislav, Bretislav, Bretik, Bronislav, Branislav, Branek, Branik, Budislav, Budek, Ceslav, Ctislav, Ctibor, Dalibor, Dobromil, Dobromir, Dobroslav, Drahomir, Draha, Drahos, Drahosek, Durko, Sudan, Sudanek, Dusek, Honza, Jarek, Jarousek, Jaromil, Jaromir, Jarek, Jaropluk, Jaroslav, Jur, Karda, Kvetoslav, Lubomir, Lubor, Lumir, Luba, Lubek, Luborek, Lubos, Lubosek, Ludomir, Ludoslav, Mecislav, Mecek, Mecik, Mecislavek, Milan, Milic, Miloslav, Milda, Milon, Milos, Miroslav, Mirek, Mstislav, Nepomuk, Pomuk, Nepomucek, Premysl, Myslik, Premek, Pribislav, Priba, Pribik, Pribisek, Radek, Radik, Radacek, Radan, Radko, Rados, Radousek, Radomir, Radimir, Radim, Radoslav, Rostislav, Rosta, Rostek, Rosticek, Rostik, Slavomir, Slava, Slavoj, Sobeslav, Sobek, Sobik, Stanislav, Stana, Standa, Stanek, Stanko, Stanicek, Stanik, Svatomir, Svatopluk, Svatoslav, Techomir, Techoslav, Veleslav, Vela, Velek, Velousek, Venceslav, Václav, Vacek, Vasek, Vena, Venousek, Wenzel, Vladimír, Vladislav, Ladislav, Vlad, Vlastimil, Vojtech, Vojta, Wojtek, Vojtik, Vojtisek, Zbyhnev, Zbyna, Zbytek, Zbytek, Zelislav, Zelek, Zelicek, Zelik, Zelousek, Zdeslav, Zdislav, Zdik Zdisek, Zitomir, Zitek, Zitousek, Zivan,Zivanek, Zivek, Zivko, Zlatan, Zlatek, Zlaticek, Zlatik, Klatko, Zlatousek";
	vector<string> names;

	ifstream names_file;
	names_file.open("names.txt", ios::in);

	if(!names_file.is_open())
		return 0;	

	while(!names_file.eof()){
		string line;
		names_file >> line;

		names.push_back(line);
	}

	MarkovNames mng(names, 3, 4);
	while(true){
		cout << mng.GetName() << endl;
		system("pause");
	}

	return 0;
}