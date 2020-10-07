#include "Actuator.h"
#include "Cloud.h"
#include "Setup.h"
#include "subsystem.h"

int main(int argc, char **argv) {
  int port, party;
  parse_party_and_port(argv, &party, &port);
  NetIO *io = new NetIO(party == ALICE ? nullptr : "127.0.0.1", port);
  setup_semi_honest(io, party);
  bool print = 0;

  subSystem *subsystem = new subSystem();
  Setup *setup = new Setup();
  Cloud *cloud = new Cloud();
  cout << "HERE" << endl;
  subsystem->inputData();
  cout << "Begins 2!!!" << endl;
  setup->inputData();
  setup->computeConstants();

  cloud->getInputs(setup->L, setup->sizeL, setup->K, setup->sizeK,
                   setup->gamma3, setup->sizegamma3, setup->gamma2,
                   setup->sizegamma2, setup->gamma1, setup->sizegamma1,
                   subsystem->xr, subsystem->sizexr, subsystem->ur,
                   subsystem->sizeur, subsystem->x0, subsystem->sizex0);
  cloud->computeConstants();

  int k = 0;
  cout << "z" << k << ":  " << endl;
  for (int i = 0; i < subsystem->sizexk[0]; i++) {
    for (int j = 0; j < subsystem->sizexk[1]; j++) {
      cout << subsystem->xk[i][j].reveal<double>(BOB) << ", ";
    }
    cout << endl;
  }

  cloud->computeuk();
  subsystem->measureState(cloud->uk);
  subsystem->computezk();

  if (print) {
    cout << "u" << 0 << ":  " << endl;
    for (int i = 0; i < cloud->sizeuk[0]; i++) {
      for (int j = 0; j < cloud->sizeuk[1]; j++) {
        cout << fixed << setprecision(5) << cloud->uk[i][j].reveal<double>(BOB)
             << ", ";
      }
      cout << endl;
    }
    cout << endl << endl;
    cout << "z" << k << ":  " << endl;
    for (int i = 0; i < subsystem->sizexk[0]; i++) {
      for (int j = 0; j < subsystem->sizexk[1]; j++) {
        cout << subsystem->zk[i][j].reveal<double>(BOB) << ", ";
      }
      cout << endl;
    }
    cout << "z" << k << "2:  " << endl;
    for (int i = 0; i < subsystem->sizexk[0]; i++) {
      for (int j = 0; j < subsystem->sizexk[1]; j++) {
        cout << subsystem->xk[i][j].reveal<double>(BOB) << ", ";
      }
      cout << endl;
    }
    cout << "xHat" << k << ":  " << endl;
    for (int i = 0; i < cloud->sizexHatk[0]; i++) {
      for (int j = 0; j < cloud->sizexHatk[1]; j++) {
        cout << cloud->xHatk[i][j].reveal<double>(BOB) << ", ";
      }
      cout << endl;
    }
    cout << endl << endl;
  }
  for (k = 1; k < 300; k++) {
    cloud->computexHat(subsystem->zk);
    cloud->computeuk();
    subsystem->measureState(cloud->uk);
    subsystem->computezk();

    if (print) {
      cout << "xHat" << k << ":  " << endl;
      for (int i = 0; i < cloud->sizexHatk[0]; i++) {
        for (int j = 0; j < cloud->sizexHatk[1]; j++) {
          cout << cloud->xHatk[i][j].reveal<double>(BOB) << ", ";
        }
        cout << endl;
      }
      cout << endl << endl;
      cout << "u" << k << ":  " << endl;
      for (int i = 0; i < cloud->sizeuk[0]; i++) {
        for (int j = 0; j < cloud->sizeuk[1]; j++) {
          cout << cloud->uk[i][j].reveal<double>(BOB) << ", ";
        }
        cout << endl;
      }
      cout << endl << endl;
      cout << "z" << k << ":  " << endl;
      for (int i = 0; i < subsystem->sizezk[0]; i++) {
        for (int j = 0; j < subsystem->sizezk[1]; j++) {
          cout << subsystem->xk[i][j].reveal<double>(BOB) << ", ";
        }
        cout << endl;
      }
      cout << endl << endl;
    }
  }

  delete io;
  return 0;
}
