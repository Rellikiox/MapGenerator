
#include "MarkovNames.h"

#include <iostream>
using namespace std;

int main(int argc, char * argv[]){

	string names = "ADITI, ADITYA, AGNI, ANANTA, ANIL, ANIRUDDHA, ARJUNA, ARUNA, ARUNDHATI, BALA, BALADEVA, BHARATA, BHASKARA, BRAHMA, BRIJESHA, CHANDRA, DAMAYANTI, DAMODARA, DEVARAJA, DEVI, DILIPA, DIPAKA, DRAUPADI, DRUPADA, DURGA, GANESHA, GAURI, GIRISHA, GOPALA, GOPINATHA, GOTAMA, GOVINDA, HARI, HARISHA, INDIRA, INDRA, INDRAJIT, INDRANI, JAGANNATHA, JAYA, JAYANTI, KALI, KALYANI, KAMA, KAMALA, KANTI, KAPILA, KARNA, KRISHNA, KUMARA, KUMARI, LAKSHMANA, LAKSHMI, LALITA, MADHAVA, MADHAVI, MAHESHA, MANI, MANU, MAYA, MINA, MOHANA, MOHINI, MUKESHA, MURALI, NALA, NANDA, NARAYANA, PADMA, PADMAVATI, PANKAJA, PARTHA, PARVATI, PITAMBARA, PRABHU, PRAMODA, PRITHA, PRIYA, PURUSHOTTAMA, RADHA, RAGHU, RAJANI, RAMA, RAMACHANDRA, RAMESHA, RATI, RAVI, REVA, RUKMINI, SACHIN, SANDHYA, SANJAYA, SARASWATI, SATI, SAVITR, SAVITRI, SHAILAJA, SHAKTI, SHANKARA, SHANTA, SHANTANU, SHIVA, SHIVALI, SHRI, SHRIPATI, SHYAMA, SITA, SRI, SUMATI, SUNDARA, SUNITA, SURESHA, SURYA, SUSHILA, TARA, UMA, USHA, USHAS, VALLI, VASANTA, VASU, VIDYA, VIJAYA, VIKRAMA, VISHNU, YAMA, YAMI";

	MarkovNames mng(names, 3);
	while(true){
		cout << mng.GetName() << endl;
		system("pause");
	}
}