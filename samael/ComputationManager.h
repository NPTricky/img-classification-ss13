#ifndef COMPUTATIONMANAGER_H_
#define COMPUTATIONMANAGER_H_

#include <QObject>
#include <SamaelImage.h>

class ComputationManager : public QObject
{
public:

  static ComputationManager* getInstance();
  Q_OBJECT
  
  SamaelImage image;
  SamaelImage temp;
  SamaelImage grey;
  IplImage* imgSurf;

  cv::KeyPoint *KeyPoint;
  void loadImg(QString imagePath);
  int SURF(QString path);
  int SIFT(QString imagePath);
  int MSER(QString imagePath);

    
private:
  
  ComputationManager();
  ~ComputationManager();

};

#endif