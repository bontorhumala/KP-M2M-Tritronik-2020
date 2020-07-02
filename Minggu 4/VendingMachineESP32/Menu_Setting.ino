void checkListPos(int choice){
  tft.fillRect(250,55,25,35*4, TFT_WHITE);
  switch (choice) {
    case 1: {
      TJpgDec.drawSdJpg(250, 55, logo + "checklist.jpg");
      break;
    }
    case 2: {
      TJpgDec.drawSdJpg(250, 55+35, logo + "checklist.jpg");
      break;
    }
    case 3: {
      TJpgDec.drawSdJpg(250, 55+70, logo + "checklist.jpg");
      break;
    }
    case 4: {
      TJpgDec.drawSdJpg(250, 55+105, logo + "checklist.jpg");
      break;
    }
    default: {
      TJpgDec.drawSdJpg(250, 55, logo + "checklist.jpg");
      break;
    }
  }
}
