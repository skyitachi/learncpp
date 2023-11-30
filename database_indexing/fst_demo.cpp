//
// Created by Shiping Yao on 2023/11/30.
//
#include <fst/fst-decl.h>
#include <fst/fstlib.h>

#include <iostream>

int main() {

  fst::StdVectorFst fst;

  fst.AddState();
  fst.SetStart(0);

  fst.AddArc(0, fst::StdArc(1, 1, 0.5, 1));
  fst.AddArc(0, fst::StdArc(2, 2, 1.5, 1));

  fst.AddState();
  fst.AddArc(1, fst::StdArc(3, 3, 2.5, 2));

  fst.AddState();
  fst.SetFinal(2, 3.5);

  fst.Write("binary.txt");
}