package org.example;

public class MatrixMultiplicationTask implements Runnable {
    private Matrix A;
    private Matrix B;
    private Matrix C;
    private int id;
    private int toMultiply;
    private int[] indexes;



    public MatrixMultiplicationTask(Matrix A, Matrix B, Matrix C, int[] indexes, int id){
        this.A = A;
        this.B = B;
        this.C = C;
        this.indexes = indexes;
        this.id = id;
    }

    public void run() {
        int startIndex = indexes[2*id];
        int stopIndex = indexes[2*id+1];
        for(int i = startIndex; i < stopIndex; i++) {
            int bCol = i % C.cols();
            int aRow = (i - bCol) / C.cols();
            //System.out.println("Thred " + id + " " + aRows + ":" + bCol);
            float sum = 0.0f;
            for(int j = 0; j< A.cols(); j++) {
                sum+= A.get(aRow, j) * B.get(j, bCol);
            }
            C.set(aRow, bCol, sum);
        }
    }
}
