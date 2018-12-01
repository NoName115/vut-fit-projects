//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - Code under test
//
// $NoKeywords: $ivs_project_1 $white_box_code.h
// $Authors:    Filip Vaverka <ivaverka@fit.vutbr.cz>
//              David Grochol <igrochol@fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file white_box_code.h
 * @author Filip Vaverka
 * @author David Grochol
 * 
 * @brief Deklarace tridy matice umoznujici zakladni maticove operace.
 */

#pragma once

#ifndef MATRIX_H_
#define MATRIX_H_

#include <utility>
#include <vector>
#include <limits>
#include <cmath>

/**
 * @brief Trida reprezuntiji matici
 * 
 */
class Matrix
{
public:
  /**
   * @brief Matrix
   * Kontruktor vytvori nulovou matici velikosti 1x1
   */
  Matrix();
  /**
   * @brief Matrix
   * Kontruktor vytvori nulovou matici velikosti row x col
   *
   * @param      row    radek matice
   * @param      col    sloupec matice
   */
  Matrix(size_t row, size_t col);

  /**
   * @brief Matrix
   * Destruktor
   */
  ~Matrix();
  /**
   * @brief      set
   *      * nastavi hodnotu v matici na pozici x,y
   *
   * @param      row    radek matice
   * @param      col    sloupec matice
   * @param      value  hodnota ulozena na pozici x,y
   *
   * @return     pokud bylo vlozeni uspesne vrati true, jinak false
   */
  bool set(size_t row, size_t col, double value);
  /**
   * @brief      set
   *      * nastavi matici hodnotami z pole
   *
   * @param      values  hodnoty pro inicializaci matice
   *
   * @return     pokud bylo vlozeni uspesne vrati true, jinak false
   */
  bool set(std::vector<std::vector< double > > values);
  /**
   * @brief      get
   *      * vrati hodnotu v matici na pozici x,y 
   * @param      row    radek matice
   * @param      col    sloupec matice
   *
   * @return     hodnota v matici na pozici x,y
   */
  double get(size_t row, size_t col);

    /**
   * @brief      porovnani
   *        * porovna obe matice
   *
   * @param      Matrix - matice pro porovnani
   *
   * @return     pokud jsou matice shodne tak vrati true, jinak false
   */
  bool operator==(const Matrix) const;

  /**
   * @brief      scitani
   *        * secte dve matice
   *
   * @param      Matrix - druhy scitanec
   *
   * @return     vysledna matice po secteni matic
   */
  Matrix operator+(const Matrix) const;

  /**
   * @brief      nasobeni
   *        * vynasobi matice
   *
   * @param      Matrix - druhy cinitel
   *
   * @return     vysledna matice po vynasobeni matic
   */
  Matrix operator*(const Matrix) const;

  /**
   * @brief      skalarni nasobeni
   *        * vynasob9 matici skalarni hodnotou
   *
   * @param      value - skalarni cinitel
   *
   * @return     vysledna matice po vynasobeni matice skalarem
   */
  Matrix operator*(const double value) const;

  /**
   * @brief      reseni spoustavy linearnich rovnic
   *        * soustava rovnic je resena pomoci cramerova pravidla
   *
   * @param      b prava strana rovnice
   *
   * @return     pole vysledku x1, x2, ...
   */
  std::vector<double> solveEquation(std::vector<double> b);





protected:
  /**
   * 2D pole reprezentujici matici
   */
  std::vector<std::vector<double> > matrix;
  /**
   * @brief      kontrola zda indexy row, col jsou v matici
   *
   * @param      row   radek matice
   * @param      col   sloupec matice
   *
   * @return     Pokud je alespon jeden index mimo matici vrati false,
   *             jinak true
   */
  bool checkIndexes(size_t row, size_t col);
  /**
   * @brief      kontrola zda maji matice shodnou velikost
   *
   * @param      m     matice pro porovnani
   *
   * @return     Pokud maji matice shodnou velikost vrati true, jinak false
   */
  bool checkEqualSize(const Matrix m) const;

  /**
   * @brief      kontrola zda je matice ctvercova
   *
   * @return     pokud je matice ctvercova tak vrati true, jinak false
   */
  bool checkSquare();
  /**
   * @brief      vypocte dereminant matice
   *
   * @return     Vrati hodnotu determinantu matice
   */
  double determinant();

  /**
   * @brief      Pomocna funkce pro vypocet determinantu matice vyssich radu
   *
   * param       m matice 
   * param       n rad matice 
   * @return     Vrati hodnotu determinantu matice
   */
  double deter(std::vector<std::vector<double> > m, size_t n);
};



#endif /* MATRIX_H_ */
