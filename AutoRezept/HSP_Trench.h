#ifndef HSP_Trench_H
#define HSP_Trench_H

class HSP_Trench {
public:
	HSP_Trench() {}
	virtual ~HSP_Trench() {}
	SetStandOrtID(int id){StandOrtID = id;}

	// nur wenn 0 OK
	int GebietsPfadPruefen(std::wstring);

private:

	int StandOrtID;
	//Von links nach rechts
	// Zeichentrenner ist das | . Kein l und keine 1 !
	// OberstesGebiet|UnteresGebiet beliebig kaskadierbar
	std::wstring Gebietspfad;

	// return 2
	int GebietsPfadSchonVergeben();
	// | nicht erlaubt return 1
	int VerbotenesZeichen();
    
};

#endif