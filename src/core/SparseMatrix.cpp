#include  <SparseMatrix.h>
#include  <iostream>
#include  <Exceptions.h>

// Initialise to zeros
SparseMatrix::SparseMatrix(int cols_, int rows_){
    fNCols = cols_;
    fNRows  = rows_; 
    fArmaMat = arma::sp_mat(fNCols, fNRows);
}

void 
SparseMatrix::SetComponent(size_t col_, size_t row_, double val_){
    if (col_ >= fNCols || row_ >= fNRows)
        throw NotFoundError(Formatter() 
                            << "Attempted out of bounds access on  matrix (" 
                            << row_ <<  "," << col_ << ")."
                            << "Matrix is (" << fNCols << "x" << fNRows
                            << ")"
                            );

    fArmaMat(col_,row_) = val_; 
}

double 
SparseMatrix::GetComponent(size_t col_, size_t row_) const{
    if (col_ >= fNCols || row_ >= fNRows)
        throw NotFoundError(Formatter() 
                            << "Attempted out of bounds access on  matrix (" 
                            << row_ <<  "," << col_ << ")."
                            << "Matrix is (" << fNCols << "x" << fNRows
                            << ")"
                            );
    return fArmaMat(col_, row_);
}

std::vector<double>
SparseMatrix::operator() (const std::vector<double>& input_) const{
    arma::vec newContents;
    try{
        // convert to armadillo vec
        newContents = fArmaMat * arma::vec(input_);
    }
    catch(const std::logic_error& e_){
        throw DimensionError(Formatter() << "DenseMatrix::opeator() : Input v\
ector ("
                             << input_.size() << ")"
                             << " wrong size for Matrix ("
                             << fNCols << "x" << fNRows
                             << " ) to act on");
    }

    // armadillo function for quick transfer to std::vector double
    return arma::conv_to<std::vector<double> >::from((newContents));
}


SparseMatrix
SparseMatrix::operator*=(const SparseMatrix& other_){
  fArmaMat = fArmaMat * other_.fArmaMat;
  return *this;
}

void
SparseMatrix::SetZeros(){
    if(!fNRows && !fNCols)
        return;
    fArmaMat = arma::sp_mat(fNCols, fNRows);
}

// FIXME: unsigned vs. size_t
void 
SparseMatrix::SetComponents(const std::vector<unsigned>& rowIndices_,
                          const std::vector<unsigned>& colIndices_,
                          const std::vector<double>& values_){
    if(rowIndices_.size() != values_.size() || colIndices_.size() != values_.size())
        throw DimensionError("SparseMatrix::SetComponent() #values != #locations");

    arma::umat locs(2, rowIndices_.size());
    locs.row(0) = arma::urowvec(rowIndices_);
    locs.row(1) = arma::urowvec(colIndices_);

    fArmaMat = arma::sp_mat(locs, arma::vec(values_));
}
