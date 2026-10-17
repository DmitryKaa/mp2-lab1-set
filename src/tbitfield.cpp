// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <climits>
#include <stdexcept>

static const int BITS_PER_ELEM = sizeof(TELEM) * CHAR_BIT;

TBitField::TBitField(int len)
{
  if (len < 0)
    throw invalid_argument("Bit field length must be non-negative");
  BitLen = len;
  MemLen = len / BITS_PER_ELEM + (len % BITS_PER_ELEM != 0);
  pMem = new TELEM[MemLen]();
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;
  pMem = new TELEM[MemLen];
  for (int i = 0; i < MemLen; i++)
    pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
  delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
  if (n < 0 || n >= BitLen)
    throw out_of_range("Bit index is out of range");
  return n / BITS_PER_ELEM;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
  return TELEM(1) << (n % BITS_PER_ELEM);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
  int index = GetMemIndex(n);
  pMem[index] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
  int index = GetMemIndex(n);
  pMem[index] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
  int index = GetMemIndex(n);
  return (pMem[index] & GetMemMask(n)) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
  if (this != &bf)
  {
    TELEM *newMem = new TELEM[bf.MemLen];
    for (int i = 0; i < bf.MemLen; i++)
      newMem[i] = bf.pMem[i];
    delete[] pMem;
    pMem = newMem;
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
  }
  return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
  if (BitLen != bf.BitLen)
    return 0;
  for (int i = 0; i < MemLen; i++)
    if (pMem[i] != bf.pMem[i])
      return 0;
  return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
  TBitField result(BitLen > bf.BitLen ? BitLen : bf.BitLen);
  for (int i = 0; i < result.MemLen; i++)
  {
    TELEM left = i < MemLen ? pMem[i] : 0;
    TELEM right = i < bf.MemLen ? bf.pMem[i] : 0;
    result.pMem[i] = left | right;
  }
  return result;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
  TBitField result(BitLen > bf.BitLen ? BitLen : bf.BitLen);
  for (int i = 0; i < MemLen && i < bf.MemLen; i++)
    result.pMem[i] = pMem[i] & bf.pMem[i];
  return result;
}

TBitField TBitField::operator~(void) // отрицание
{
  TBitField result(BitLen);
  for (int i = 0; i < BitLen; i++)
    if (!GetBit(i))
      result.SetBit(i);
  return result;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
  // Биты вводятся от индекса 0 до GetLength() - 1, например: 0101.
  TBitField result(bf.BitLen);
  for (int i = 0; i < bf.BitLen; i++)
  {
    char bit;
    if (!(istr >> bit))
      return istr;
    if (bit != '0' && bit != '1')
    {
      istr.setstate(ios::failbit);
      return istr;
    }
    if (bit == '1')
      result.SetBit(i);
  }
  bf = result;
  return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
  for (int i = 0; i < bf.BitLen; i++)
    ostr << bf.GetBit(i);
  return ostr;
}