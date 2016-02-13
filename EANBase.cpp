#include "StdAfx.h"
#include "eanbase.h"

CEANBase::CEANBase (int nType)
{
	m_nType = nType;

	// set A - normal 
	// set B - reverse
	// set C - negative
	m_strEAN[0] = "3211";
	m_strEAN[1] = "2221";
	m_strEAN[2] = "2122";
	m_strEAN[3] = "1411";
	m_strEAN[4] = "1132";
	m_strEAN[5] = "1231";
	m_strEAN[6] = "1114";
	m_strEAN[7] = "1312";
	m_strEAN[8] = "1213";
	m_strEAN[9] = "3112";

	// select A or B set for EAN-13
	m_strSet[0] = "AAAAAA";
	m_strSet[1] = "AABABB"; //"BBABAA";	// conj.
	m_strSet[2] = "AABBAB"; //"BABBAA"; // conj.
	m_strSet[3] = "AABBBA"; //"ABBBAA";	// conj.
	m_strSet[4] = "ABAABB"; //"BBAABA";
	m_strSet[5] = "ABBAAB"; //"AABAAB"; //"BAABBA";		//test!!
	m_strSet[6] = "ABBBAA"; //"AABBBA";	// conj.
	m_strSet[7] = "ABABAB"; //"BABABA";
	m_strSet[8] = "ABABBA"; //"ABBABA";	// conj.
	m_strSet[9] = "ABBABA"; //"ABABBA";

	m_strSetAddon2[0] = "AA";
	m_strSetAddon2[1] = "AB";
	m_strSetAddon2[2] = "BA";
	m_strSetAddon2[3] = "BB";

	m_strSetAddon5[0] = "BBAAA";
	m_strSetAddon5[1] = "BABAA";
	m_strSetAddon5[2] = "BAABA";
	m_strSetAddon5[3] = "BAAAB";
	m_strSetAddon5[4] = "ABBAA";
	m_strSetAddon5[5] = "AABBA";
	m_strSetAddon5[6] = "AAABB";
	m_strSetAddon5[7] = "ABABA";
	m_strSetAddon5[8] = "ABAAB";
	m_strSetAddon5[9] = "AABAB";

	// standard sizes for EAN barcodes
	m_eansize[0] = new CEANSize (0.80f,  0.0f,  35); // print quality in micrometers
	m_eansize[1] = new CEANSize (0.90f,  0.0f,  68);
	m_eansize[2] = new CEANSize (1.00f,  3.8f, 100);
	m_eansize[3] = new CEANSize (1.10f,  4.6f, 116);
	m_eansize[4] = new CEANSize (1.20f,  5.3f, 131);
	m_eansize[5] = new CEANSize (1.40f,  6.8f, 162);
	m_eansize[6] = new CEANSize (1.50f,  7.6f, 178);
	m_eansize[7] = new CEANSize (1.70f,  9.1f, 209);
	m_eansize[8] = new CEANSize (1.85f, 10.2f, 232);
	m_eansize[9] = new CEANSize (2.00f, 11.4f, 255);

	// default font, foreground, and background
	m_strFont = "Verdana";
	m_colForegroundColor = RGB (0x00, 0x00, 0x00);
	m_nBackgroundMode = OPAQUE;
	m_colBackgroundColor = RGB (0xff, 0xff, 0xff);

	SetCode ("");
}

CEANBase::~CEANBase(void)
{
	for (int i = 0; i < 10; i++)
		delete m_eansize[i];
}

void CEANBase::SetCode (CString strCode, CString strAddon)
{
	m_strCode = strCode;
	// fill up with leading zeros
	for (int i = strCode.GetLength (); i < m_nType; i++)
		m_strCode = "0" + m_strCode;

	m_strAddon = strAddon;

	m_strCode.AppendFormat ("%d", CalcCheckDigit ());

	if (strAddon.IsEmpty ())
		m_nAddonType = 0;
	else
		m_nAddonType = strAddon.GetLength () > 2 ? 5 : 2;	// addon size - 2 or 5 digits
}

// calculate check digit
int CEANBase::CalcCheckDigit ()
{
	int nWeights[] = {0, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3};
	int nSum = 0;

	for (int i = 1; i < m_nType; i++)
		nSum += nWeights[i] * (m_strCode.GetAt (i - 1) - '0');

	return (10 - (nSum % 10)) % 10;
}

// explode packed sets of bar
CString CEANBase::Explode (int nDigit, char chSet)
{
	CString strPack = m_strEAN[nDigit];
	if (chSet == 'B')
	{
		_strrev (strPack.GetBuffer (-1));
		strPack.ReleaseBuffer ();
	}

	char chFirst = chSet == 'C' ? '1' : '0';
	CString strResult;

	for (int i = 0; i < strPack.GetLength (); i++)
	{
		for (int j = 0; j < strPack.GetAt (i) - '0'; j++)
			strResult += chFirst;
		chFirst = chFirst == '1' ? '0' : '1';
	}
	
	return strResult;
}
