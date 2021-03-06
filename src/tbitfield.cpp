// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include "String"

TBitField::TBitField(int len){
	if (len>0){
	BitLen=len;
	MemLen=BitLen/sizeof(TELEM)+1;
	pMem=new TELEM[MemLen];
	memset(pMem,0,MemLen*4);
	}
	else throw len;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen=bf.BitLen;
	MemLen=bf.MemLen;
	pMem=new TELEM[MemLen];
	memcpy(pMem,bf.pMem,MemLen*4);
}

TBitField::~TBitField()
{
	delete [] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if ((n>=0) && (n<BitLen))
	return (n/sizeof(TELEM));
	else throw n;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return (1<<(n%sizeof(TELEM)));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n>=0) && (n<BitLen))
	pMem[GetMemIndex(n)]|=GetMemMask(n);
	else throw n;
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n>=0) && (n<BitLen))
	pMem[GetMemIndex(n)]&=~GetMemMask(n);
	else throw n;
}

int TBitField::GetBit(const int n) const // получить значение бита
{ if ((n>=0) && (n<BitLen))
  return (GetMemMask(n)&pMem[GetMemIndex(n)]);
  else throw n;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if(MemLen!=bf.MemLen) {
		delete[] pMem;
		MemLen=bf.MemLen;
		pMem=new TELEM[MemLen];
	}
	BitLen=bf.BitLen;
	for(int i=0;i<MemLen;i++)
		pMem[i]=bf.pMem[i];
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen!=bf.BitLen) return 0;
	for (int i=0;i<MemLen-1;i++)
		if (pMem[i]!=bf.pMem[i]) return 0;
	for(int i=BitLen-1;i>BitLen-BitLen%32;i++)
		if (GetBit(i)!=bf.GetBit(i)) return 0;
  return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  if (BitLen!=bf.BitLen) return 1;
	for (int i=0;i<MemLen-1;i++)
		if (pMem[i]!=bf.pMem[i]) return 1;
	for(int i=BitLen-1;i>BitLen-BitLen%32;i++)
		if (GetBit(i)!=bf.GetBit(i)) return 1;
  return 0;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int size,sizeM;
	if (BitLen>bf.BitLen){
		size=BitLen;
		sizeM=bf.MemLen;
		}
	else 
	{ size=bf.BitLen;
	  sizeM=MemLen;
	}
	TBitField tmp(size);
	for (int i=0;i<sizeM;i++)
		tmp.pMem[i]=pMem[i]|bf.pMem[i];
	for (int i=sizeM;i<size/32;i++)
		tmp.pMem[i]=bf.pMem[i];
	return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int size,sizeM;
	if (MemLen>bf.MemLen) {
		size=bf.BitLen;
		sizeM=BitLen;
	}
	else {
		size=BitLen;
		sizeM=bf.BitLen;
	}
   TBitField tmp(sizeM);
   for(int i=0;i<size/32;i++)
	   tmp.pMem[i]=pMem[i]&bf.pMem[i];
   for(int i=size;i<sizeM;i++)
	   tmp.ClrBit(i);
   return tmp;

}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField tmp(BitLen);
	for(int i=0;i<MemLen;i++)
		tmp.pMem[i]=~pMem[i];
	return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	string st;
	istr>>st;
	for(int i=0;i<st.size();i++){
	if((st[i] != '1') && (st[i] != '0')) break;
		if (st[i]=='1')	bf.SetBit(i);
		else bf.ClrBit(i);
	} 
    return istr;
	

}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for(int i=0;i<bf.BitLen;i++)
		if (bf.GetBit(i)!=0) ostr<<1;
		else ostr<<0;
	return ostr<<endl;
}
