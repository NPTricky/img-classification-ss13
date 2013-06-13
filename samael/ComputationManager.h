#ifndef COMPUTATIONMANAGER_H_
#define COMPUTATIONMANAGER_H_

#include <QObject>
#include <SamaelImage.h>
#include <vector>

class ComputationManager : public QObject
{
public:

  static ComputationManager* getInstance();
  Q_OBJECT

  int SURF(QString path);
  std::vector<cv::DMatch> SIFT(std::string imagePath);
  int MSER(QString imagePath);

    
private:
  
  ComputationManager();
  ~ComputationManager();

};

#endif