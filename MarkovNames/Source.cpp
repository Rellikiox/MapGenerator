
#include "MarkovNames.h"

#include <iostream>
using namespace std;

int main(int argc, char * argv[]){

	string names = "Berislava, Biserka, Blaga, Blagica, Bla�enka, Bogdana, Bogomila, Bogumila, Borka, Borislava, Bo�ena, Bo�ica, Bo�idarka, Branimira, Branka, Buga, Cvita, Cvijeta, ?ika, Danica, Davorka, Divna, Dragana, Dragica, Dra�enka, Dubravka, Dunja, Hrvoja, Hrvojka, Jasenka, Jasna, Ljuba, Ljubica, Mila, Milica, Miljenka, Mislava, Mira, Mirka, Mirna, Mojmira, Morana, Nada, Neda, Nediljka, Nevenka, Ognjenka, Ranka, Ra�eljka, Ratka, Ru�a, Ru�ica, Sanja, Slava, Slavica, Slavenka, Smiljana, Spomenka, Srebrenka, Stanislava, Stana, Stanka, Snje�ka, Snje�ana, Sun?ana, Sun?ica, Svitlana, Svjetlana, Teha, Tehana, Tihana, Tihomila, Tuga, Vedrana, Vera, Verica, Vjera, Vesna, Vjekoslava, Vlasta, Vlatka, Zdenka, Zlata, Zora, Zorica, Zorka, Zrinka, Zrina, Zvjezdana, Zvonimira, Zvonka, �eljka, �ivka, Berislav, Berivoj, Blago, Bogdan, Bogumil, Bogoljub, Bogomil, Boris, Borislav, Borna, Bo�etjeh, Bo�idar, Bo�o, Bratislav, Budimir, Branimir, Brajko, Branko, Braslav, Bratoljub, Cvitko, Cvjetko, ?aslav, ?astimir, ?edomir, Dalibor, Damir, Darko, Davor, Desimir, Dobroslav, Dobrovit, Domagoj, Dragan, Drago, Dragoslav, Dragutin, Dra�an, Dra�en, Dr�iha, Dr�islav, Godemir, Gojko, Gojislav, Gojslav, Goran, Grubi�a, Hrvatin, Hrvoj, Hrvoje, Hrvoslav, Kazimir, Ka�imir, Jasenko, Klonimir, Kre�imir, Kr�evan, Lavoslav, Ljubomir, Ljudevit, Milan, Mile, Milivoj, Milovan, Miljenko, Mirko, Miroslav, Miro�, Mislav, Mladen, Mojmir, Mutimir, Nediljko, Nedjeljko, Nenad, Ognjen, Ostoja, Ozren, Predrag, Pribislav, Prvan, Prvoslav, Prvo�, Radimir, Radomir, Rado�, Rajko, Ranko, Ratimir, Ratko, Rato, Radovan, Radoslav, Slaven, Slavi�a, Slavoljub, Slavomir, Smiljan, Spomenko, Srebrenko, Sre?ko, Stanislav, Stanko, Strahimir, Svetoslav, Tihomil, Tje�imir, Tomislav, Tomo, Tvrtko, Trpimir, Vatroslav, Ve?eslav, Velimir, Veselko, Vidoslav, Vjekoslav, Vjenceslav, Vi�eslav, Vitomir, Vjeran, Vladimir, Vlado, Vlatko, Vojmil, Vojnomir, Vuk, Zdenko, Zdeslav, Zdravko, Zorislav, Zoran, Zrinko, Zrinoslav, Zlatko, Zvonimir, Zvonko, �arko, �elimir, �eljko, �ivko";

	MarkovNames mng(names, 3, 4);
	while(true){
		cout << mng.GetName() << endl;
		system("pause");
	}
}