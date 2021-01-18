void startSequence() {
  sequence.pattern = patterns[(int)SEQUENCES[currSeq][0]];
  sequence.iterations = (int)SEQUENCES[currSeq][1];
  sequence.doReverse = 0;
  sequence.speedStart = SEQUENCES[currSeq][2];
  sequence.speedEnd = SEQUENCES[currSeq][3];
  sequence.p1s = SEQUENCES[currSeq][4];
  sequence.p1e = SEQUENCES[currSeq][5];
  sequence.p2s = SEQUENCES[currSeq][6];
  sequence.p2e = SEQUENCES[currSeq][7];
  sequence.p3s = SEQUENCES[currSeq][8];
  sequence.p3e = SEQUENCES[currSeq][9];
  sequence.p4s = SEQUENCES[currSeq][10];
  sequence.p4e = SEQUENCES[currSeq][11];
  sequence.p5s = SEQUENCES[currSeq][12];
  sequence.p5e = SEQUENCES[currSeq][13];
  sequence.start();
}
