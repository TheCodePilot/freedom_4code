#ifndef RR_SYSTEM_H
#define RR_SYSTEM_H

#include <Windows.h> //BSTR
#include <iostream>
#include <map>
#include <string>
#include <vector>
	//#include <comdef.h>  // _bstr_t    Probleme ohne das auskommen
#include <msxml6.h>  // IXMLDOMNode      !!!!GANZ WICHTIG!!!!
	//#pragma comment(lib, "msxml6.lib")

#include <memory>
#include <vcl.h>

//bcc32 akzeptiert kein nullptr, kein shared_ptr, keine Initialliste bei Konstruktoren, kein auto
namespace RRTools
{

	//Zeichenketten Konverter
	static AnsiString WStrToAnsiString(std::wstring wstr)
	{
		AnsiString ansi(wstr.c_str());
		return ansi;

	}

	static std::wstring AnsiToWide(const AnsiString ansi)
	{
		std::string str = ansi.c_str();
		std::wstring wstr = std::wstring(str.begin(), str.end());
		return wstr;
	}

	static std::string AnsiToString(const AnsiString ansi)
	{
		std::string str = ansi.c_str();
		return str;
	}

	static std::wstring BSTRToWString(const BSTR bs)
	{
		assert(bs != NULL);
		std::wstring wstr(bs, SysStringLen(bs));
		return wstr;
	}

	static std::wstring WStringToBSTR(const std::wstring wstr)
	{
		assert(!wstr.empty());
		BSTR bs = SysAllocStringLen(wstr.data(), wstr.size());
		return bs;
	}

	//!!! Achtung aber daran denken wieder mit SysFreeString(MyBSTR) frei geben.
	//BSTR MyBSTR = RRTools::StringToBSTR(L"Test"); //was Code... if(MyBSTR){SysFreeString(MyBSTR);}
	static BSTR StringToBSTR(const std::wstring& str)
	{
		return SysAllocStringLen(str.c_str(), str.length());
	}

	static wchar_t* StringToWCHAR(const std::wstring& str)
	{
		return const_cast<wchar_t*>(str.c_str());
	}

}// namespace RRTools

namespace RRClasses{

	//Test-KLasse ob C++ Builder seine Arbeit macht
	class Parent
	{
		public:
			Parent(){ShowMessage("Konstruktor: Ich bin Parent");}
			virtual ~Parent(){ShowMessage("Destruktor: Ich bin Parent");}
			virtual void IchBin()const{ShowMessage("Ich bin Parent");}
			void IchBinParentAberNutzbar()const{ShowMessage("IchBinParentAberNutzbar : Ich bin eine Funktion von Parent");}

	};

	class Child : public Parent
	{
		public:
			Child(){ShowMessage("Konstruktor: Ich bin Child");}
			~Child(){ShowMessage("Destruktor: Ich Bin Child");}
			void IchBinEigenstaendigesChild()const{ShowMessage("Ich bin ein Eigenstaendiges Child");}
			void IchBin()const{ShowMessage("Obwohl diese Funktion vererbt: Ich bin Child");}
	};

	class MoreThanCpp03
	{
		MoreThanCpp03(){ShowMessage("Destruktor: Ich Bin Mit Unique_ptr allokiert und brauche kein delete !");}
	};


    //class 4 XML-Handling

	class XMLBasic {

	public:
		XMLBasic();
		virtual ~XMLBasic();

		VARIANT_BOOL LoadXMLDocument(const std::wstring& filename);
		IXMLDOMDocument3* GetDocOfDOM()const {
			return  pXMLDom;
		}

	protected:
		IXMLDOMDocument3* pXMLDom;
		//std::shared_ptr<IXMLDOMDocument3> pXMLDom;  wird nicht mit bcc32 unterstützt :(
		bool isInitializedCOM;
		void ReleaseResources();

	};

	class XMLData;

    class BasicNode
	{
	public:
		BasicNode();
		~BasicNode();

		IXMLDOMNode* GetTheNodeIam()const
		{
			return Iam;
		}

		void SetGuiEnable(bool wish)
		{
			guiEnable = wish;
		}

		bool GetGuiEnable()const
		{
			return guiEnable;
		}

		void SetGuiVisible(bool wish)
		{
			guiVisible = wish;
		}

		bool GetGuiVisible()const
		{
			return guiVisible;
		}

		void SetCustomSpecific(std::wstring wish)
		{
			custom = wish;
		}

		std::wstring GetCustomSpecific()const
		{
			return custom;
		}

		friend class RRClasses::XMLData;

		//Anderen zur Zeit keine Relevanz
		//später dazu wenn Zeit. Für diese Projekt nicht nötig

	private:

		void SetNodeIam(IXMLDOMNode* node) {
			Iam = node;
		}

		IXMLDOMNode* Iam;

		bool IamTheRoot;
		bool HaveChilds;
		bool HaveSiblings;

		bool guiEnable;
		bool guiVisible;
		std::wstring custom;
	};

	class XMLData : public RRClasses::XMLBasic
	{
	public:
		XMLData();
		~XMLData();
		void DummyIfThis(){if(!this){ShowMessage("THIS::XMLData Ich bin NULL !");}}
		int AnalyzeDOM_PicEventUwish(std::wstring ereignis, std::wstring datum, int option = 0);
//______

		//--------------------------
		void addNode(IXMLDOMNode* node) {
			nodes.push_back(node);
		}

		IXMLDOMNode* GetNode(size_t index)const;

		size_t GetInfoCount() const {
			return xmlInfo.size();
		}

		size_t GetNodeCount() const {
			return nodes.size();
		}
//--------------------------
		virtual const std::vector<std::pair<std::wstring, std::wstring> >& GetXMLInfo()const;
		
		const std::vector<std::pair<BasicNode, int> >& GetXMLInfo4Gui() const {
			return xmlInfo4Gui;
		}

		std::pair<std::wstring, std::wstring> GetNodeInfo(IXMLDOMNode* pNode)const;

		std::wstring GetCommentIfType18() const {
			return commentIfType18;
		}
//--------------------------
		HRESULT AnalyzeDOM();

		virtual int AnalyzeDOM_PicAllEvents();
		//Aufruf in der VCL-Anwendung
		virtual int CollectCards_Items();
		int FindTheMeasurementData(IXMLDOMNodeList* pSiblings,int option = 0);
        int FindTheMeasurementDataStrCat(IXMLDOMNodeList* pSiblings,int option = 0);

		virtual int AnalyzeDOM_ProofTyp(size_t coiceInList);

		//-------------TEST
		//Maximale Anzahl verschiedener Messdaten-Arten
		int countMaxDifferM;
		void WriteInfos();
		//-------------TEST

		//später privat
		//Speichert Pärchen von Info. Beispiele Knotenname und Wert als Text. Deweiteren aber auch nutzbar wie im speziellen Fall: Ereignis (Messung oder Kommentar) und Datum
		std::vector < std::pair < std::wstring, std::wstring > > xmlInfo4UserChoice;
		std::vector < std::pair < std::wstring, std::wstring > > xmlInfo;

		private:
		HRESULT IniRoot();
		void ReleaseResources();
		void ClearTheNodes();
		void ClearTheInfo();
		void ClearTheInfo4GUI();

		void CreateGuiNode(IXMLDOMNode* node);

		void addBasicNode(const std::pair<BasicNode, int>& node4Gui)
		{
			xmlInfo4Gui.push_back(node4Gui);
		}


		IXMLDOMElement* pRootNode;
		IXMLDOMNode* findTheNode__Card_Item; //(1)
		//Speichert Nodes
		std::vector<IXMLDOMNode*> nodes;

		//Fuer die GUI
		std::vector < std::pair < BasicNode, int > > xmlInfo4Gui;

		std::string strError;
		std::wstring commentIfType18;

	 };

	//Functor für pairs
	template <typename Typ1, typename Typ2>
	class RRFTorPairs
	{
	public:
		RRFTorPairs(Typ1 cond1, Typ2 cond2) : condition1(cond1), condition2(cond2) {}

		bool operator()(const std::pair<Typ1, Typ2>& p)
		{
			return p.first == condition1 && p.second == condition2;
		}

	private:
		Typ1 condition1;
		Typ2 condition2;
	};

	template <typename Typ1, typename Typ2>
	class RRFTor1Condition
	{
	public:
		RRFTor1Condition(Typ1 cond1, Typ2 cond2) : condition1(cond1), condition2(cond2) {}

		bool operator()(const std::pair<Typ1, Typ2>& p)
		{
			return ((condition1 > -1) && p.second == condition2);
		}

	private:
		Typ1 condition1;
		Typ2 condition2;

	};

    template <typename Typ1, typename Typ2>
	class RRFTor1Condition2
	{
	public:
		RRFTor1Condition2(Typ1 cond1, Typ2 cond2) : condition1(cond1), condition2(cond2) {}

		bool operator()(const std::pair<Typ1, Typ2>& p)
		{
			return ( (p.first == condition1  ) && (condition2 > -1) );
		}

	private:
		Typ1 condition1;
		Typ2 condition2;

	};




} //namespace RRClasses

#endif  // RR_SYSTEM

/*
ZU 1:
//Wichtiger Zeiger auf die rictige Cards-Item
//vector vecCard_Item mit allen cards jeweils mit einer Card_Item- Instanz der Klasse Card_Item
*/



