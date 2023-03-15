package org.example;


import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Arrays;

public class App {
    public static void main( String[] args ) {
        String aFile = ".\\A.txt";
        String bFile = ".\\B.txt";
        int nThreads = 3;
        ArrayList<Thread> threads = new ArrayList<>();
        int[] indexes = new int[nThreads*2];
        try {
            Matrix A = new Matrix(aFile);
            Matrix B = new Matrix(bFile);
            System.out.println("A matrix:");
            A.print();
            System.out.println("B matrix:");
            B.print();
            int toFill = A.rows() * B.cols();
            int rest = toFill % nThreads;
            int evenToFillPerThread = (toFill-rest)/nThreads;
            for(int i = 0; i < nThreads; i++) {
                int howMany = rest > i ? evenToFillPerThread + 1 : evenToFillPerThread;
                if(i==0) {
                    indexes[2 * i] = 0;
                    indexes[2 * i + 1] = howMany;
                } else {
                    indexes[2 * i] = indexes[2 * (i-1) + 1];
                    indexes[2 * i + 1] = indexes[2 * i] + howMany;
                }
                if(indexes[2 * i + 1]>toFill) {
                    indexes[2 * i + 1] = toFill;
                }
            }
            Matrix C = new Matrix(A.rows(), B.cols());

            for(int i = 0; i < nThreads; i++) {
                Runnable task = new MatrixMultiplicationTask(A, B, C, indexes, i);
                threads.add(new Thread(task, "Thread" + i));
            }
            long startThread = System.nanoTime();
            for(int i = 0; i < nThreads; i++) {
                threads.get(i).start();
            }

            for(int i = 0; i< nThreads; i++) {
                threads.get(i).join();
            }
            long stopThread = System.nanoTime();

            long startClassic = System.nanoTime();
            Matrix D = mult(A,B);
            long stopClassic = System.nanoTime();
            System.out.println("C matrix(thread):");
            C.print();
            System.out.println("Time elapsed(thread): " + (stopThread-startThread));
            System.out.println("C matrix(classic):");
            D.print();
            System.out.println("Time elapsed(classic): " + (stopClassic-startClassic));
        } catch (FileNotFoundException | InterruptedException e) {
            throw new RuntimeException(e);
        }
    }

    public static Matrix mult(Matrix A, Matrix B) {
        Matrix C = new Matrix(A.rows(), B.cols());

        for (int r = 0; r < A.rows(); r++) {
            for (int c = 0; c < B.cols(); c++) {
                float s = 0;
                for (int k = 0; k < A.cols(); k++) {
                    s += A.get(r,k) * B.get(k, c);
                }
                C.set(r,c,s);
            }
        }

        return C;
    }
}
