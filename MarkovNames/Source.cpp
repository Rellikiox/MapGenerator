
#include "MarkovNames.h"

#include <iostream>
using namespace std;

int main(int argc, char * argv[]){

	string names = "Berislava, Biserka, Blaga, Blagica, Blaženka, Bogdana, Bogomila, Bogumila, Borka, Borislava, Božena, Božica, Božidarka, Branimira, Branka, Buga, Cvita, Cvijeta, ?ika, Danica, Davorka, Divna, Dragana, Dragica, Draženka, Dubravka, Dunja, Hrvoja, Hrvojka, Jasenka, Jasna, Ljuba, Ljubica, Mila, Milica, Miljenka, Mislava, Mira, Mirka, Mirna, Mojmira, Morana, Nada, Neda, Nediljka, Nevenka, Ognjenka, Ranka, Rašeljka, Ratka, Ruža, Ružica, Sanja, Slava, Slavica, Slavenka, Smiljana, Spomenka, Srebrenka, Stanislava, Stana, Stanka, Snješka, Snježana, Sun?ana, Sun?ica, Svitlana, Svjetlana, Teha, Tehana, Tihana, Tihomila, Tuga, Vedrana, Vera, Verica, Vjera, Vesna, Vjekoslava, Vlasta, Vlatka, Zdenka, Zlata, Zora, Zorica, Zorka, Zrinka, Zrina, Zvjezdana, Zvonimira, Zvonka, Željka, Živka, Berislav, Berivoj, Blago, Bogdan, Bogumil, Bogoljub, Bogomil, Boris, Borislav, Borna, Božetjeh, Božidar, Božo, Bratislav, Budimir, Branimir, Brajko, Branko, Braslav, Bratoljub, Cvitko, Cvjetko, ?aslav, ?astimir, ?edomir, Dalibor, Damir, Darko, Davor, Desimir, Dobroslav, Dobrovit, Domagoj, Dragan, Drago, Dragoslav, Dragutin, Dražan, Dražen, Držiha, Držislav, Godemir, Gojko, Gojislav, Gojslav, Goran, Grubiša, Hrvatin, Hrvoj, Hrvoje, Hrvoslav, Kazimir, Kažimir, Jasenko, Klonimir, Krešimir, Krševan, Lavoslav, Ljubomir, Ljudevit, Milan, Mile, Milivoj, Milovan, Miljenko, Mirko, Miroslav, Miroš, Mislav, Mladen, Mojmir, Mutimir, Nediljko, Nedjeljko, Nenad, Ognjen, Ostoja, Ozren, Predrag, Pribislav, Prvan, Prvoslav, Prvoš, Radimir, Radomir, Radoš, Rajko, Ranko, Ratimir, Ratko, Rato, Radovan, Radoslav, Slaven, Slaviša, Slavoljub, Slavomir, Smiljan, Spomenko, Srebrenko, Sre?ko, Stanislav, Stanko, Strahimir, Svetoslav, Tihomil, Tješimir, Tomislav, Tomo, Tvrtko, Trpimir, Vatroslav, Ve?eslav, Velimir, Veselko, Vidoslav, Vjekoslav, Vjenceslav, Višeslav, Vitomir, Vjeran, Vladimir, Vlado, Vlatko, Vojmil, Vojnomir, Vuk, Zdenko, Zdeslav, Zdravko, Zorislav, Zoran, Zrinko, Zrinoslav, Zlatko, Zvonimir, Zvonko, Žarko, Želimir, Željko, Živko";

	MarkovNames mng(names, 3, 4);
	while(true){
		cout << mng.GetName() << endl;
		system("pause");
	}
}