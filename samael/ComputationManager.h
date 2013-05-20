#ifndef COMPUTATIONMANAGER_H_
#define COMPUTATIONMANAGER_H_

#include <QObject>

class ComputationManager : public QObject
{
public:

  static ComputationManager* getInstance();
  Q_OBJECT
  
  cv::KeyPoint *KeyPoint;
  int SURF();

    
private:
  
  ComputationManager();
  ~ComputationManager();

};

#endif