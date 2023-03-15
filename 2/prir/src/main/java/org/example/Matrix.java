package org.example;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Locale;
import java.util.Scanner;

public class Matrix {
    private int ncols;
    private int nrows;
    private float[] _data;

    public Matrix(int r, int c) {
        this.ncols = c;
        this.nrows = r;
        _data = new float[c*r];
    }
    public Matrix(String fname) throws FileNotFoundException {
        File f = new File(fname);
        Scanner scanner = new Scanner(f).useLocale(Locale.ENGLISH);

        this.nrows  = scanner.nextInt();
        this.ncols  = scanner.nextInt();
        _data = new float[this.ncols*this.nrows];

        for (int r = 0; r < this.nrows; r++) {
            for (int c = 0; c < this.ncols; c++) {
                float v = scanner.nextFloat();
                this.set(r, c, v);
            }
        }
    }

    public float get(int r, int c) {
        return _data[r*ncols + c];
    }

    public void set(int r, int c, float v) {
        _data[r*ncols +c] = v;
    }

    public int rows() {
        return nrows;
    }

    public int cols() {
        return ncols;
    }

    public void print() {
        System.out.println("[");
        for (int r = 0; r < nrows; r++) {
            for (int c = 0; c < ncols; c++) {
                System.out.print(this.get(r,c));
                System.out.print(" ");
            }
            System.out.println("");
        }
        System.out.println("]");
    }

}
