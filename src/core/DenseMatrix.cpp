#include <DenseMatrix.h>
#include <iostream>
#include <Exceptions.h>
#include <Formatter.hpp>
#include <math.h> //sqrt

// initalise to zeros
DenseMatrix::DenseMatrix(int rows_, int cols_){
    fNCols = cols_;
    fNRows = rows_;
    fArmaMat = arma::mat(rows_, cols_, arma::fill::zeros);
}

void 
DenseMatrix::SetComponent(size_t row_, size_t col_, double val_){
    if (col_ >= fNCols || row_ >= fNRows)
        throw NotFoundError(Formatter()
                            << "Attempted out of bounds access on  matrix ("
                            << row_ <<  "," << col_ << ")."
                            << "Matrix is (" << fNRows << "x" << fNCols
                            << ")"
                            );
            
    fArmaMat(row_,col_) = val_;
}

double 
DenseMatrix::GetComponent(size_t row_, size_t col_) const{
    if (col_ >= fNCols || row_ >= fNRows)
        throw NotFoundError(Formatter()
                            << "Attempted out of bounds access on  matrix ("
                            << row_ <<  "," << col_ << ")."
                            << "Matrix is (" << fNRows << "x" << fNCols
                            << ")"
                            );
    return fArmaMat(row_, col_);
}

std::vector<double>
DenseMatrix::operator() (const std::vector<double>& input_) const{  
    arma::vec newContents;
    try{
        // convert to armadillo vec
        newContents = fArmaMat * arma::vec(input_);
    }
    catch(const std::logic_error& e_){
        throw DimensionError(Formatter() << "DenseMatrix::opeator() : Input vector ("
                                         << input_.size() << ")" 
                                         << " wrong size for Matrix ("
                                         << fNRows << "x" << fNCols 
                                         << " ) to act on");
    }

    // armadillo function for quick transfer to std::vector double
    return arma::conv_to<std::vector<double> >::from((newContents));
}

DenseMatrix
DenseMatrix::operator*=(const DenseMatrix& other_){
  fArmaMat = fArmaMat * other_.fArmaMat;
  return *this;
}

void
DenseMatrix::SetZeros(){
    if(!fNRows || !fNCols)
        throw DimensionError(Formatter()<<
                "DenseMatrix:: Can't set elements to zero. (rows,cols) : ("<<
                fNRows<<","<<fNCols<<")"  
                );
    fArmaMat.zeros();
}

void
DenseMatrix::SetToIdentity(){
    if(!fNRows || !fNCols)        
        throw DimensionError(Formatter()<<
                "DenseMatrix:: Can't set identity as matrix is not square. (rows,cols) : ("<<
                fNRows<<","<<fNCols<<")"  
                );
    fArmaMat.eye();
}

void
DenseMatrix::SetSymmetricMatrix(const std::vector<double>& _input){
    int noVectorEntries = _input.size();
    double testTriangular = (sqrt((8*noVectorEntries) + 1) - 1) / 2;

    if (testTriangular != floor(testTriangular))
        throw DimensionError(Formatter()
                            << "Input vector to create symmetric matrix "
                            << "must have triangular number of entries."
			    << "Vector has " << noVectorEntries 
			    << " entries."
                            );

    if (testTriangular != fNRows)
        throw DimensionError(Formatter()
                            << "Number of entries in input vector "
                            << "does not match number of rows in matrix."
			    << "Vector has " << noVectorEntries 
			    << " entries. Matrix has " << fNRows
                            ); 

    if (fNRows != fNCols)
        throw DimensionError(Formatter()
                            << "Symmetric matrix must be square."
                            << "This is a  (" << fNCols << "x" << fNRows 
                            << " ) matrix."
			    );

    int i = 0;
    while(i < noVectorEntries)
      {
        for(int j = 0; j < fNRows; j++)
          {
            for(int k = 0;  k < j; k++)
              {
                fArmaMat(k, j) = _input.at(i);
                fArmaMat(j, k) = _input.at(i);
                i++;  
	      }
	  }        
      }
}
