// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
  if (len <= 0)
    throw(-2);
  
  BitLen = len;
  MemLen = (len - 1) / (sizeof(TELEM) * 8) + 1;
  pMem = new TELEM[MemLen];

  for (int i = 0; i < MemLen; ++i)
    pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;
  pMem = new TELEM[MemLen];

  for (int i = 0; i < MemLen; ++i)
    pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
  delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
  if (n < 0 || n >= BitLen)
    throw(-1);

  return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
  if (n < 0 || n >= BitLen)
    throw(-1);

  return 1 << (n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
  pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
  pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
  return (pMem[GetMemIndex(n)] & GetMemMask(n)) == 0 ? 0 : 1;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
  if (this != &bf)
  {
    delete[] pMem;

    BitLen = bf.BitLen;
    MemLen = bf.MemLen;

    pMem = new TELEM[MemLen];

    for (int i = 0; i < MemLen; ++i)
      pMem[i] = bf.pMem[i];
  }

  return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
  if (this == &bf)
    return 1;

  if (BitLen != bf.BitLen)
    return 0;

  for (int i = 0; i < MemLen - 1; ++i)
    if (pMem[i] != bf.pMem[i])
      return 0;

  for (int i = (MemLen - 1) * sizeof(TELEM) * 8; i < BitLen; ++i)
    if (GetBit(i) != bf.GetBit(i))
      return 0;

  return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !operator==(bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
  const TBitField &min = BitLen < bf.BitLen ? *this : bf;
  const TBitField &max = BitLen < bf.BitLen ? bf : *this;
  int minlen = min.GetLength();
  TBitField ret(max);

  for (int i = 0; i < minlen; ++i)
    if (min.GetBit(i))
      ret.SetBit(i);

  return ret;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
  const TBitField &min = BitLen < bf.BitLen ? *this : bf;
  const TBitField &max = BitLen < bf.BitLen ? bf : *this;
  int minlen = min.GetLength();
  TBitField ret(max.GetLength());

  for (int i = 0; i < minlen; ++i)
    if (min.GetBit(i) && max.GetBit(i))
      ret.SetBit(i);

  return ret;
}

TBitField TBitField::operator~(void) // отрицание
{
  TBitField ret(BitLen);

  for (int i = 0; i < BitLen; ++i)
    if (!GetBit(i))
      ret.SetBit(i);

  return ret;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
  TBitField res(bf);
  int len = bf.GetLength() + 1;
  char *input = new char[len];

  istr.getline(input, len);

  for (int i = len - 2, j = 0; i >= 0; --i, ++j)
  {
    if (input[i] == '0')
    {
      bf.ClrBit(j);
    }
    else if (input[i] == '1')
    {
      bf.SetBit(j);
    }
    else
    {
      bf = res;
      delete[] input;
      throw(-3);
    }
  }

  delete[] input;

  return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
  for (int i = bf.GetLength() - 1; i >= 0; --i)
    ostr << bf.GetBit(i);

  return ostr;
}
