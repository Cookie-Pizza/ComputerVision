#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <QDialog>
#include "kfc.h"

namespace Ui {
class MainFrame;

}

class ImageForm;
class KVoronoiDgm;
class KPotentialField;

class MainFrame : public QDialog
{
    Q_OBJECT

private:
    Ui::MainFrame *ui;

    KPtrList<ImageForm*>*   _plpImageForm;
    ImageForm*              _q_pFormFocused;


public:
    explicit MainFrame(QWidget *parent = 0);
    ~MainFrame();

    void            ImageFormFocused(ImageForm* q_pImageForm)
                    {   _q_pFormFocused  = q_pImageForm;   //활성화된 창의 포인터를 저장함
                        UpdateUI();                        //UI 활성화 갱신
                    }
    void            UpdateUI();
    void            CloseImageForm(ImageForm* pForm);

public:
    void            OnMousePos(const int& nX, const int& nY, ImageForm* q_pForm);

private slots:
    void on_buttonOpen_clicked();
    void on_buttonDeleteContents_clicked();    
    void on_buttonSepiaTone_clicked();
    void on_buttonShowList_clicked();

    void on_buttonLuminance_clicked();

    void on_OtsuLabel_clicked();

    void on_GaussianFilter_clicked();

    void on_BoxFilter_clicked();

    void on_MedianFilter_clicked();

    void on_HarrisCorner_clicked();

    void on_Hough_clicked();

    void on_Otical_BTN_clicked();

    void on_NEXT_BTN_clicked();

    void on_Btn_ImgBlending_clicked();

protected:
    void closeEvent(QCloseEvent* event);
};

#endif // MAINFRAME_H
