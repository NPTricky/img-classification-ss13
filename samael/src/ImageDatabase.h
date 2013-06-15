#ifndef IMAGEDATABASE_H_
#define IMAGEDATABASE_H_

#include <map>
#include <vector>

#include <QObject>

class SamaelImage;

/*
Saves all Images with their corresponding class names.
It also manages which images have been used for training and the images which needs to be classified.
So if you need to train the classificator, just call "getTrainingImages" with the apropriate class name and you get a vector with the images.
If you need to classify unclassified images, just call "getClassifyImages" and you get all the images which weren't used in the trainign step.
*/

class ImageDataBase : public QObject
{
public:

  static ImageDataBase* getInstance();

  Q_OBJECT

private:

  ImageDataBase();
  ~ImageDataBase();

  std::vector<QString> m_classNames;//all the class names, useful to iterate through the complete database
  std::map<QString, std::vector<SamaelImage*>> m_images;//saves the images in respect to their class
  std::map<QString, std::vector<int>> m_trainingImageIndices;//saves the indices of the images in a class which have been used for training

public slots:

  void getClassNames(std::vector<QString> out_classNames);

  void getTrainingImages(QString className, std::vector<SamaelImage*> &out_images);
  void getClassifyImages(QString className, std::vector<SamaelImage*> &out_images);

  void addImage(QString className, SamaelImage *image);
  void removeImages(QString className);
};

#endif