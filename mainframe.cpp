
#include <QFileDialog>
#include <QPainter>

#include "mainframe.h"
#include "ui_mainframe.h"
#include "imageform.h"
#include <QStack>
#include <iostream>
#include <stdio.h>
#include <math.h>
using namespace std;
int page_count = 1;
ImageForm* p_fix;


KImageColor& KImageColor::MedianFiltered(int nWindow)
{
#ifdef _DEBUG
    assert(_ppA !=0 );
#endif

    //buffering
    KImageColor igTmp(*this);

    //median filtering
    int		nHalf = nWindow/2;
    int		nMid  = _SQR((nHalf<<1)+1)/2;
    KVector	vItem_R(_SQR((nHalf<<1)+1)),dTmp;
    KVector	vItem_G(_SQR((nHalf<<1)+1));
    KVector	vItem_B(_SQR((nHalf<<1)+1));

    for(int i=nHalf; i<Row()-nHalf; i++){
        for(int j=nHalf; j<Col()-nHalf; j++)
        {
            for(int m=-nHalf,k=0; m<=nHalf; m++){
                for(int n=-nHalf; n<=nHalf; n++){
                    vItem_R[k] = igTmp._ppA[i+m][j+n].r;
                    vItem_G[k] = igTmp._ppA[i+m][j+n].g;
                    vItem_B[k] = igTmp._ppA[i+m][j+n].b;
                    k++;
                }
            }
            //sort
            vItem_R.Sort(_ASCEND);
            vItem_G.Sort(_ASCEND);
            vItem_B.Sort(_ASCEND);
            //set the median value
            _ppA[i][j].r = (unsigned char)vItem_R[nMid];
            _ppA[i][j].g = (unsigned char)vItem_G[nMid];
            _ppA[i][j].b= (unsigned char)vItem_B[nMid];
        }
    }


    return *this;
}



KImageColor& KImageColor::BoxFiltering(double &dSigma){
    int     i,j,r,c,rr,cc;

    //create the conv. mask
    int     nHalf = (int)(3.0*dSigma+0.3);
    KMatrix mMask(nHalf*2 + 1,nHalf*2 + 1);

    //obtain the mask
    double  dScale = 0.0;


    for(r = -nHalf,i=0; r <= nHalf; r++,i++)
    {
        for(c = -nHalf,j=0; c <= nHalf; c++,j++)
        {
            mMask[i][j] = 1;
            dScale     += mMask[i][j];
        }
    }
    mMask /= dScale;

    //convolution
    KImageColor icTmp(*this);
    double      dR,dG,dB;
    int         nSy = nHalf, nEy = Row() - nHalf;
    int         nSx = nHalf, nEx = Col() - nHalf;

    for(i=nSy; i<nEy; i++)
    {
        for(j=nSx; j<nEx; j++)
        {
            //convolution
            dR = dG = dB = 0.0;
            for(r = -nHalf,rr=0; r <= nHalf; r++,rr++)
            {
                for(c = -nHalf,cc=0; c <= nHalf; c++,cc++)
                {
                    dR += icTmp._ppA[i+r][j+c].r  * mMask[rr][cc];
                    dG += icTmp._ppA[i+r][j+c].g * mMask[rr][cc];
                    dB += icTmp._ppA[i+r][j+c].b * mMask[rr][cc];
                }
            }
            _ppA[i][j].r = (unsigned char)(dR);
            _ppA[i][j].g = (unsigned char)(dG);
            _ppA[i][j].b = (unsigned char)(dB);
        }
    }

    return *this;
}

int Labeling(const KImageGray *ap_src,unsigned char *ap_dst)
{
    int height = ap_src->Row();
    int width = ap_src->Col();

    unsigned char *inputImage = new unsigned char[height*width];
    int *resultImage = new int[height*width];



    for(int y = 0; y<height ; y++){
        for(int x = 0; x<width ; x++){
            inputImage[width*y + x ] = (*ap_src)[y][x];

            resultImage[width*y + x ] = 0;
        }
    }

    QStack<QPoint> st;
    int labelNumber = 0;

    for(int y=1; y<height -1;y++){
        for(int x=1 ; x<width -1; x++){
            if(inputImage[width*y + x ] !=255 || resultImage[width*y + x ]!=0)continue;

            labelNumber++;

            st.push(QPoint(x,y));

            while(!st.empty()){
                int ky = st.top().y();
                int kx = st.top().x();
                st.pop();

                resultImage[width*ky + kx] = labelNumber;

                for(int ny = ky-1; ny<= ky+1 ; ny++){
                    if(ny <0 || ny>= height) continue;
                    for(int nx = kx-1; nx<= kx+1; nx++){
                        if(nx<0 || nx>= width) continue;

                        if(inputImage[width*ny + nx] != 255 || resultImage[width*ny + nx] != 0) continue;
                        st.push(QPoint(nx,ny));

                        resultImage[width*ny + nx] = labelNumber;
                    }
                }
            }
        }
    }

    for(int y = 0; y<height ; y++){
        for(int x = 0; x<width ; x++){
            ap_dst[width * y + x] = resultImage[width * y + x];
        }
    }

    delete[] inputImage;
    delete[] resultImage;

    return labelNumber;
}


class SM_Label{
private:
    KCOLOR24 m_RGB;
    unsigned char m_label_id;
    KPOINT m_point;

public:
    SM_Label(){
        m_RGB.r = qrand()%255;
        m_RGB.g = qrand()%255;
        m_RGB.b = qrand()%255;
        m_label_id = 0;
    }
    void SetID(unsigned char parm_id){
        m_label_id = parm_id;
    }
    unsigned char GetID(){return m_label_id;}
    void SetPoint(int a_x, int a_y){m_point.nX = a_x; m_point.nY = a_y;}
    KPOINT GetPoint(){return m_point;}
    KCOLOR24 GetRGB(){
        return m_RGB;
    }
};



MainFrame::MainFrame(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainFrame)
{
    ui->setupUi(this);

    _plpImageForm       = new KPtrList<ImageForm*>(10,false,false);
    _q_pFormFocused     = 0;

    //객체 맴버의 초기화

    //get a current directory
    char st[100];
    GetCurrentDirectoryA(100,st);

    //리스트 출력창을 안보이게
    ui->listWidget->setVisible(false);
    this->adjustSize();

    //UI 활성화 갱신
    UpdateUI();
}

MainFrame::~MainFrame()
{ 
    delete ui;
    delete _plpImageForm;


}

void MainFrame::CloseImageForm(ImageForm *pForm)
{
    //ImageForm 포인터 삭제
    _plpImageForm->Remove(pForm);

    //활성화 ImageForm 초기화
    _q_pFormFocused     = 0;

    //관련 객체 삭제

    //UI 활성화 갱신
    UpdateUI();
}

void MainFrame::UpdateUI()
{
    if(ui->tabWidget->currentIndex() == 0)
    {
        ui->buttonSepiaTone->setEnabled( _q_pFormFocused &&  _q_pFormFocused->ID() == "OPEN" );
        ui->buttonLuminance->setEnabled( _q_pFormFocused &&  _q_pFormFocused->ID() == "OPEN" );

    }
    else if(ui->tabWidget->currentIndex() == 1)
    {

    }
    else if(ui->tabWidget->currentIndex() == 2)
    {

    }
}

void MainFrame::OnMousePos(const int &nX, const int &nY, ImageForm* q_pForm)
{

    UpdateUI();
}

void MainFrame::closeEvent(QCloseEvent* event)
{
    //생성된 ImageForm을 닫는다.
    for(int i=_plpImageForm->Count()-1; i>=0; i--)
        _plpImageForm->Item(i)->close();

    //리스트에서 삭제한다.
    _plpImageForm->RemoveAll();
}


void MainFrame::on_buttonOpen_clicked()
{
    //이미지 파일 선택
    QFileDialog::Options    q_Options   =  QFileDialog::DontResolveSymlinks  | QFileDialog::DontUseNativeDialog; // | QFileDialog::ShowDirsOnly
    QString                 q_stFile    =  QFileDialog::getOpenFileName(this, tr("Select a Image File"),  "./data", "Image Files(*.bmp *.ppm *.pgm *.png)",0, q_Options);

    if(q_stFile.length() == 0)
        return;

    //이미지 출력을 위한 ImageForm 생성
    ImageForm*              q_pForm   = new ImageForm(q_stFile, "OPEN", this);

    _plpImageForm->Add(q_pForm);
    q_pForm->show();

    //UI 활성화 갱신
    UpdateUI();
}

void MainFrame::on_buttonDeleteContents_clicked()
{
    //생성된 ImageForm을 닫는다.
    for(int i=_plpImageForm->Count()-1; i>=0; i--)
        _plpImageForm->Item(i)->close();

    //리스트에서 삭제한다.
    _plpImageForm->RemoveAll();
}

void MainFrame::on_buttonSepiaTone_clicked()
{
    //포커스 된 ImageForm으로부터 영상을 가져옴
    KImageColor   icMain;
    if(_q_pFormFocused != 0 && _q_pFormFocused->ImageColor().Address() &&  _q_pFormFocused->ID() == "OPEN")
        icMain = _q_pFormFocused->ImageColor();
    else
        return;

    //hue, saturation 값 가져오기
    double dHue = ui->spinHue->text().toDouble();
    double dSat = ui->spinSaturation->text().toDouble();

    //
    //icMain 변환
    //:
    KArray<KHSV> *arHSV = new KArray<KHSV>;
    icMain.RGBtoHSV(*arHSV);
    for(int i=icMain.Row(),ii=0; i; i--,ii++){

        for(int j=icMain.Col(),jj=0; j; j--,jj++)
        {
            (*arHSV)[ii][jj].h = dHue;
            (*arHSV)[ii][jj].s = dSat;
        }
    }

    icMain.FromHSV(*arHSV);

    delete arHSV;




    //출력을 위한 ImageForm 생성
    ImageForm*  q_pForm = new ImageForm(icMain, "Sepia Tone", this);

    _plpImageForm->Add(q_pForm);
    q_pForm->show();

    //UI 활성화 갱신
    UpdateUI();
}


void MainFrame::on_buttonShowList_clicked()
{
    static int nWidthOld = ui->tabWidget->width();

    if(ui->listWidget->isVisible())
    {
        nWidthOld = ui->listWidget->width();
        ui->listWidget->hide();
        this->adjustSize();
    }
    else
    {
        ui->listWidget->show();
        QRect q_rcWin = this->geometry();

        this->setGeometry(q_rcWin.left(), q_rcWin.top(), q_rcWin.width()+nWidthOld, q_rcWin.height());
    }
}

void MainFrame::on_buttonLuminance_clicked()
{
    int nA = ui->spinA->value();
    int nB = ui->spinB->value();


    //포커스 된 ImageForm으로부터 영상을 가져옴
    KImageGray   igMain;        KImageColor   icMain;
    KImageGray   igResult;      KImageColor   icResult;
    ImageForm*  q_pForm_result;

    if(_q_pFormFocused != 0 && _q_pFormFocused->ImageGray().Address() &&  _q_pFormFocused->ID() == "OPEN")
    {
        igMain = _q_pFormFocused->ImageGray();

        KImageGray igout = igMain.ContrastTransform(nA,nB);

        q_pForm_result = new ImageForm(igout, "Contrast", this);
    }
    else if(_q_pFormFocused != 0 && _q_pFormFocused->ImageColor().Address() &&  _q_pFormFocused->ID() == "OPEN")
    {
        icMain = _q_pFormFocused->ImageColor();

        KImageGray igLumin = icMain.ColorToGray();

        KImageGray igout = igLumin.ContrastTransform(nA,nB);

        q_pForm_result = new ImageForm(igout, "Contrast", this);
    }

    else
        return;


    //출력을 위한 ImageForm 생성
    _plpImageForm->Add(q_pForm_result);
    q_pForm_result->show();

    //UI 활성화 갱신
    UpdateUI();
}

void MainFrame::on_OtsuLabel_clicked()
{
    //포커스 된 ImageForm으로부터 영상을 가져옴
    KImageGray   igMain;        KImageColor   icMain;
    KImageGray   igResult;      KImageColor   icResult;
    ImageForm*  q_pForm_result;

    if(_q_pFormFocused != 0 && _q_pFormFocused->ImageGray().Address() &&  _q_pFormFocused->ID() == "OPEN")
    {
        igMain = _q_pFormFocused->ImageGray();
        KImageGray igout;
        KHisto otsu;
        otsu.Ostu(&igMain, &igout);
        KImageColor icout;
        icout = igout.GrayToRGB();

        int height = igout.Row(), width = igout.Col();

        unsigned char *p_dest = new unsigned char[height*width]; //Labeling information

        int labelNumber = Labeling(&igout, p_dest); //the number of Label


        SM_Label *p_label = new SM_Label[labelNumber+1];


        for(int y = 0; y<height ; y++){
            for(int x = 0; x<width ; x++){
                if(p_dest[y*width + x]){
                    icout._ppA[y][x].r = p_label[p_dest[y*width + x]].GetRGB().r;
                    icout._ppA[y][x].g = p_label[p_dest[y*width + x]].GetRGB().g;
                    icout._ppA[y][x].b = p_label[p_dest[y*width + x]].GetRGB().b;
                }
            }
        }


        delete[] p_label;
        delete[] p_dest;
        q_pForm_result = new ImageForm(icout, "Labeling", this);
    }
    else if(_q_pFormFocused != 0 && _q_pFormFocused->ImageColor().Address() &&
            _q_pFormFocused->ID() == "OPEN")
    {
        icMain = _q_pFormFocused->ImageColor();

        KImageGray igMain = icMain.ColorToGray();

        KImageGray igout;
        KHisto otsu;
        otsu.Ostu(&igMain, &igout);

        KImageColor icout;
        icout = igout.GrayToRGB();
        int height = igout.Row(), width = igout.Col();

        unsigned char *p_dest = new unsigned char[height*width]; //Labeling information

        int labelNumber = Labeling(&igout, p_dest); //the number of Label


        SM_Label *p_label = new SM_Label[labelNumber+1];


        for(int y = 0; y<height ; y++){
            for(int x = 0; x<width ; x++){
                if(p_dest[y*width + x]){
                    icout._ppA[y][x].r = p_label[p_dest[y*width + x]].GetRGB().r;
                    icout._ppA[y][x].g = p_label[p_dest[y*width + x]].GetRGB().g;
                    icout._ppA[y][x].b = p_label[p_dest[y*width + x]].GetRGB().b;
                }
            }
        }


        delete[] p_label;
        delete[] p_dest;

        q_pForm_result = new ImageForm(icout, "Labeling", this);

    }

    else
        return;

    //출력을 위한 ImageForm 생성
    _plpImageForm->Add(q_pForm_result);
    q_pForm_result->show();

    //UI 활성화 갱신
    UpdateUI();
}

void MainFrame::on_GaussianFilter_clicked()
{
    //포커스 된 ImageForm으로부터 영상을 가져옴
    KImageGray   igMain;        KImageColor   icMain;
    KImageGray   igResult;      KImageColor   icResult;
    ImageForm*  q_pForm_result;

    double dsigma = ui->SpinSigma->text().toDouble();

    if(_q_pFormFocused != 0 && _q_pFormFocused->ImageColor().Address() &&  _q_pFormFocused->ID() == "OPEN")
    {
        icMain = _q_pFormFocused->ImageColor();

        icResult = icMain.GaussianSmoothed(dsigma);



    }else return;
    q_pForm_result = new ImageForm(icResult, "Gaussian", this);

    //출력을 위한 ImageForm 생성
    _plpImageForm->Add(q_pForm_result);
    q_pForm_result->show();

    //UI 활성화 갱신
    UpdateUI();

}

void MainFrame::on_BoxFilter_clicked()
{
    //포커스 된 ImageForm으로부터 영상을 가져옴
    KImageGray   igMain;        KImageColor   icMain;
    KImageGray   igResult;      KImageColor   icResult;
    ImageForm*  q_pForm_result;

    double dsigma = ui->SpinSigma->text().toDouble();

    if(_q_pFormFocused != 0 && _q_pFormFocused->ImageColor().Address() &&  _q_pFormFocused->ID() == "OPEN")
    {
        icMain = _q_pFormFocused->ImageColor();

        icResult = icMain.BoxFiltering(dsigma);



    }else return;
    q_pForm_result = new ImageForm(icResult, "BoxFiltering", this);

    //출력을 위한 ImageForm 생성
    _plpImageForm->Add(q_pForm_result);
    q_pForm_result->show();

    //UI 활성화 갱신
    UpdateUI();

}

void MainFrame::on_MedianFilter_clicked()
{
    //포커스 된 ImageForm으로부터 영상을 가져옴
    KImageGray   igMain;        KImageColor   icMain;
    KImageGray   igResult;      KImageColor   icResult;
    ImageForm*  q_pForm_result;

    double dsigma = ui->SpinSigma->text().toDouble();
    int     nHalf = (int)(3.0*dsigma+0.3);
    int     nWindow = 2*nHalf + 1;

    if(_q_pFormFocused != 0 && _q_pFormFocused->ImageColor().Address() &&  _q_pFormFocused->ID() == "OPEN")
    {
        icMain = _q_pFormFocused->ImageColor();

        icResult = icMain.MedianFiltered(nWindow);



    }else return;
    q_pForm_result = new ImageForm(icResult, "MedianFiltering", this);

    //출력을 위한 ImageForm 생성
    _plpImageForm->Add(q_pForm_result);
    q_pForm_result->show();

    //UI 활성화 갱신
    UpdateUI();

}

void MainFrame::on_HarrisCorner_clicked()
{
    //포커스 된 ImageForm으로부터 영상을 가져옴
    KImageGray   igMain;        KImageColor   icMain;
    KImageGray   igResult;      KImageColor   icResult;
    ImageForm*  q_pForm_result;

    //    double dsigma = ui->SpinSigma->text().toDouble();
    //    int     nHalf = (int)(3.0*dsigma+0.3);
    //    int     nWindow = 2*nHalf + 1;
    KCorner harriscorner(1,10);
    if(_q_pFormFocused != 0 && _q_pFormFocused->ImageGray().Address() &&  _q_pFormFocused->ID() == "OPEN")
    {
        igMain = _q_pFormFocused->ImageGray();

        harriscorner.HarrisCorner(0.5, igMain);

        icResult = igMain.GrayToRGB();




    }else return;

    q_pForm_result = new ImageForm(icResult, "Harris Coner", this);

    int count = harriscorner.Count();

    while(count){
        int u, v;

        u = harriscorner[count -1].u;
        v = harriscorner[count -1].v;

        count--;

        QPoint temp(u,v);

        q_pForm_result->DrawEllipse(temp, 5,5,0x00ff0000);

    }


    //출력을 위한 ImageForm 생성
    _plpImageForm->Add(q_pForm_result);
    q_pForm_result->show();

    //UI 활성화 갱신
    UpdateUI();
}

void MainFrame::on_Hough_clicked()
{

    //포커스 된 ImageForm으로부터 영상을 가져옴
    KImageGray   igMain;        KImageColor   icMain;
    KImageGray   igEdge;      KImageColor   icResult;
    ImageForm*  q_pForm_result;
    KArray<double*> _arVotes;
    int count = 0;
    int first_var, nDir;
    int nNumGridCy = 150, nNumGridCx = 150, nGridAng = 0, nNumGridAng = 360;
    int nGridCx, nGridCy;
    double dTmp, dDist, dAng, dCx, dCy,_dSx, _dSy;
    //double dMinRAD = -1.6580627894, dSaRAD = 0.01745329252;
    double dMinRAD = 0.0, dSaRAD = 1.0;

    int sum_x = 0 , sum_y = 0;


    KMath oMath;
    FILE *file = fopen("C:\\Homeworks_CV\\hw06.txt", "r");
    fscanf(file, "%d", &first_var);


    KPtrList<KPoint*> _lPoints2D;
    for(int i = 0; i< first_var; i++){
        int X, Y;


        fscanf(file, "%d" "%d" , &X, &Y);

        sum_x += X;
        sum_y += Y;



        _lPoints2D.Add(new KPoint((double)X,(double)Y));
    }
    fclose(file);
    KPoint *ptpOri = new KPoint(sum_x/first_var, sum_y/first_var);
    KPtrList<KPoint*> _lpTables[4];

    _lpTables[0].RemoveAll();
    _lpTables[1].RemoveAll();
    _lpTables[2].RemoveAll();
    _lpTables[3].RemoveAll();

    for(int j = 1, jj= _lPoints2D.Count() -2; jj; j++,jj--){
        dTmp = oMath.Atan((float)(_lPoints2D[j+1]->_dX - _lPoints2D[j-1]->_dX),
                (float)(_lPoints2D[j+1]->_dY - _lPoints2D[j-1]->_dY));
        dTmp += 90.0;
        nDir = ((((int)(dTmp / 22.5)+1)>>1) & 0x00000003);

        dDist = sqrt(_SQR(ptpOri->_dX-_lPoints2D[j]->_dX)+_SQR(ptpOri->_dY - _lPoints2D[j]->_dY));

        dAng = atan2(_lPoints2D[j]->_dY - ptpOri->_dY, _lPoints2D[j]->_dX- ptpOri->_dX);

        _lpTables[nDir].Add(new KPoint(dDist, dAng));
    }






    KEDGEPIXEL oEdge;

    if(_q_pFormFocused != 0 && _q_pFormFocused->ImageGray().Address() &&  _q_pFormFocused->ID() == "OPEN")
    {
        igMain = _q_pFormFocused->ImageGray();


    }else return;

    if(_arVotes.Pointer()){
        for(int i = 0; i<_arVotes.Row() ; i++){
            for(int j = 0; j<_arVotes.Col() ; j++){
                delete[] _arVotes[i][j];
            }
        }
    }

    _arVotes.Create(nNumGridCy, nNumGridCx);
    for(int i = 0; i< nNumGridCy ; i++){
        for(int j = 0; j< nNumGridCx ; j++){
            _arVotes[i][j] = new double[nNumGridAng];
            memset(_arVotes[i][j], 0 , nNumGridAng*sizeof(double));
        }
    }


    KRect _rcOri(50,50,300,300);
    _dSx = (nNumGridCx == 1? 0 :(_rcOri._nRight - _rcOri._nLeft)/(double)(nNumGridCx -1));
    _dSy = (nNumGridCy == 1? 0 :(_rcOri._nBottom - _rcOri._nTop)/(double)(nNumGridCy -1));
    KEdge _oEdge(2.0);
    _oEdge.Canny(50.0,60.0,igMain);

    while(_oEdge.Pop(oEdge) != -1){

        dAng = dMinRAD;


        for(int nGridAng = 1, nn = nNumGridAng-2; nn; nGridAng++, nn--){
            dAng += dSaRAD;
            dTmp = oEdge.wAng - dAng;
            oEdge.wDir = ((((int)(dTmp/22.5)+1)>>1) & 0x00000003); //에지 방향(0~3)

            for(int i=0, ii=_lpTables[oEdge.wDir].Count(); ii; i++,ii--) {
                dCx = oEdge.u - _lpTables[oEdge.wDir][i]->_dX * cos(_lpTables[oEdge.wDir][i]->_dY+_RADIAN(dAng));
                dCy = oEdge.v - _lpTables[oEdge.wDir][i]->_dX * sin(_lpTables[oEdge.wDir][i]->_dY+_RADIAN(dAng));

                nGridCx = (int)((dCx - _rcOri._nLeft)/_dSx);
                if(nGridCx < 1 || nGridCx > nNumGridCx -2 )
                    continue;
                nGridCy = (int)((dCy - _rcOri._nTop)/_dSy);
                if(nGridCy < 1 || nGridCy > nNumGridCy -2 )
                    continue;

                for(int i = -1; i<2 ; i++){
                    for(int j = -1; j<2 ; j++){
                        for(int k = -1 ; k< 2 ; k++){
                            _arVotes[nGridCy + i][nGridCx + j][nGridAng +k] += 0.7;
                        }
                    }
                }

                _arVotes[nGridCy][nGridCx][nGridAng] += 0.3;
            }
        }
    }

    int nGmax = 0;

    for(int i = 0; i< nNumGridCy ; i++){
        for(int j = 0; j< nNumGridCx ; j++){
            for(int k =0 ; k< nNumGridAng ; k++){
                if(_arVotes[i][j][k] > nGmax){
                    nGmax = (int)(_arVotes[i][j][k]);
                    nGridCx = j; nGridCy = i; nGridAng = k;

                }
            }
        }
    }
    //    printf("%d", (int)(nGridAng*dSaRAD*180/_PI));
    //    if(nGmax <25){
    //        return 0;
    //    }

    icResult = igMain.GrayToRGB();



    q_pForm_result = new ImageForm(icResult, "Hough", this);


    QPoint plug_center((int)(nGridCx*_dSx+_rcOri._nLeft),(int)(nGridCy*_dSy+_rcOri._nTop));
    //    int X_Dist = ptpOri->_dX - plug_center.x();
    //    int Y_Dist = ptpOri->_dY - plug_center.y();
    //    for(int i = 0 ; i< _lPoints2D.Count() ; i++){
    //        QPoint temp_point((int)(_lPoints2D[i]->_dX) - X_Dist,(int)(_lPoints2D[i]->_dY) - Y_Dist);
    //        q_pForm_result->DrawEllipse(temp_point, 5,5,0x00ff0000);

    //    }

    for(int i = 0;i<4 ; i++){
        for(int j = 0; j<_lpTables[i].Count() ; j++){
            int modified_X = plug_center.x() + (int)(_lpTables[i][j]->_dX*cos((_lpTables[i][j]->_dY)+_RADIAN((nGridAng)*dSaRAD)));
            int modified_Y = plug_center.y() + (int)(_lpTables[i][j]->_dX*sin((_lpTables[i][j]->_dY)+_RADIAN((nGridAng)*dSaRAD)));

            QPoint temp_point(modified_X, modified_Y);
            q_pForm_result->DrawEllipse(temp_point, 1,1,0x0000ff00);


        }
    }

    //q_pForm_result->DrawEllipse(plug_center, 5,5,0x00ff0000);

    KCIRCLEHOUGH_INFO Info;

    Info.nLeft = 300;
    Info.nRight = 600;
    Info.nTop = 50;
    Info.nBottom = 300;

    Info.nNumGridCx = 150;
    Info.nNumGridCy = 150;

    if(_arVotes.Pointer()){
        for(int i = 0;i<_arVotes.Row() ; i++){
            for(int j = 0; j<_arVotes.Col() ; j++){
                delete[] _arVotes[i][j];
            }
        }
    }

    _arVotes.Create(Info.nNumGridCy, Info.nNumGridCx);

    for(int i = 0; i<Info.nNumGridCy ; i++){
        for(int j = 0; j<Info.nNumGridCx ; j++){
            _arVotes[i][j] = new double;
            memset(_arVotes[i][j],0,sizeof(double));
        }
    }

    _dSx = (Info.nNumGridCx==1 ? 0 : (Info.nRight - Info.nLeft)/(double)(Info.nNumGridCx-1));
    _dSx = (Info.nNumGridCy==1 ? 0 : (Info.nBottom - Info.nTop)/(double)(Info.nNumGridCy-1));
    //_dSr = (Info.nNumGridR==1 ? 0 : (Info.nMaxR - Info.nMinR)/(double)(Info.nNumGridR-1));
    KPoint ptCen;
    while(_oEdge.Pop(oEdge) != -1){

        dAng = _RADIAN(oEdge.wAng);

        ptCen._dX = oEdge.u - 51.5*cos(dAng);
        ptCen._dY = oEdge.v - 51.5*sin(dAng);

        nGridCx = (int)((ptCen._dX - (double)Info.nLeft)/_dSx);
        nGridCy = (int)((ptCen._dY - (double)Info.nTop)/_dSy);

        for(int i = -1; i<2 ; i++){
            for(int j = -1; j< 2 ; j++){
                *(_arVotes[nGridCy + i][nGridCx + j]) += 0.7;
            }
        }

        *(_arVotes[nGridCy][nGridCx]) += 0.3;

    }
    nGmax = 0;

    for(int i = 0; i< Info.nNumGridCy ; i++){
        for(int j = 0; j< Info.nNumGridCx ; j++){

            if(*(_arVotes[i][j]) > nGmax){
                nGmax = (int)(*(_arVotes[i][j]));
                nGridCx = j; nGridCy = i;

            }
        }
    }
    QPoint Circle_center((int)(nGridCx * _dSx + Info.nLeft), (int)(nGridCy*_dSy + Info.nTop));
    q_pForm_result->DrawEllipse(Circle_center, 51.5,51.5,0x00ff0000);





    //출력을 위한 ImageForm 생성
    _plpImageForm->Add(q_pForm_result);
    q_pForm_result->show();

    //UI 활성화 갱신
    UpdateUI();

}

void MainFrame::on_Otical_BTN_clicked()
{
    ImageForm* q_pForm;
    ImageForm*  q_pForm_result;
    KImageGray igStart;
    q_pForm = new ImageForm("C:\\Homeworks_CV\\yos.01.pgm", "start", this);
    igStart = q_pForm->ImageGray();
    q_pForm_result = new ImageForm(igStart, "Hough", this);
    _plpImageForm->Add(q_pForm_result);
    q_pForm_result->show();
    page_count = 1;
    //UI 활성화 갱신
    UpdateUI();



}

void MainFrame::on_NEXT_BTN_clicked()
{

    //_q_pFormFocused->hide();
    //포커스 된 ImageForm으로부터 영상을 가져옴
    KImageGray   igMain;        KImageColor   icMain;
    KImageGray   igResult;      KImageColor   icResult;
    KImageGray   igMain_prev;
    ImageForm*  q_pForm_result;
    ImageForm*  q_pForm;

    KImageGray igMain_cur;
    QString str1, str2, str3;
    str1 = "C:\\Homeworks_CV\\yos.0";
    str2 = page_count + '0';
    str3 = ".pgm";


    //"C:\\Homeworks_CV\\yos.01.pgm"
    if(page_count < 9){
        //str4 = "yos.0" + page_count + '0';
        q_pForm = new ImageForm(str1 + str2 + str3, "yos.0" + page_count + '0', this);
        igMain_prev = q_pForm->ImageGray();
        delete q_pForm;
        page_count++;
        str2 = page_count + '0';
        q_pForm = new ImageForm(str1 + str2 + str3, "yos.0" + page_count + '0',this);
        igMain_cur = q_pForm->ImageGray();
        delete q_pForm;
    }else if(page_count == 9){
        q_pForm = new ImageForm("C:\\Homeworks_CV\\yos.09.pgm", "yos.0" + page_count + '0', this);
        igMain_prev = q_pForm->ImageGray();
        delete q_pForm;
        page_count = 10;

        q_pForm = new ImageForm("C:\\Homeworks_CV\\yos.10.pgm", "yos.10",this);
        igMain_cur = q_pForm->ImageGray();
        delete q_pForm;
    }else if(page_count > 9 && page_count < 16){
        str1 = "C:\\Homeworks_CV\\yos.";
        str2 = (char)(page_count/10 + '0');
        QString str4;
        str4 = (char)(page_count%10 + '0');
        q_pForm = new ImageForm(str1 + str2 +str4 + str3, "yos." + str2 + str4,this);
        igMain_prev = q_pForm->ImageGray();
        delete q_pForm;
        page_count++;
        if(page_count == 17) return;
        str2 = (char)(page_count/10 + '0');
        str4 = (char)(page_count%10 + '0');
        q_pForm = new ImageForm(str1 + str2 +str4 + str3, "yos.0" + str2+ str4,this);
        igMain_cur = q_pForm->ImageGray();
        delete q_pForm;
    }else return;

    KArray<int> Intensity_X;
    KArray<int> Intensity_Y;
    KArray<int> Intensity_T;

    Intensity_X.Create(igMain_prev.Row() , igMain_prev.Col());
    Intensity_Y.Create(igMain_prev.Row(), igMain_prev.Col());
    Intensity_T.Create(igMain_prev.Row(), igMain_prev.Col());

    Intensity_X.Clear();
    Intensity_Y.Clear();
    Intensity_T.Clear();


    for(int i = 1; i< igMain_prev.Row() ; i++){
        for(int j = 1; j< igMain_prev.Col(); j++){
            Intensity_X[i][j] = igMain_prev._ppA[i][j] -igMain_prev._ppA[i][j-1];
            Intensity_Y[i][j] = igMain_prev._ppA[i][j] -igMain_prev._ppA[i-1][j];
            Intensity_T[i][j] = (-1)*(igMain_cur._ppA[i][j] - igMain_prev._ppA[i][j]);
        }
    }

    for(int i = 0; i< igMain_prev.Row() ; i++) {
        for(int j = 0; j< igMain_prev.Col() ; j++){
            cout << (int)(Intensity_X[i][j]) << " ";
        }
    }

    icResult = igMain_cur.GrayToRGB();
    q_pForm_result = new ImageForm(icResult,"yos.0" + page_count + '0' , this);

    int window_x = 0, window_y = 0;

    KMatrix W(25,25);
    for(int i = 0; i< 25 ; i++){
        for(int j = 0; j< 25; j++){
            W[i][i] = 0;
        }
    }

    W[0][0] = 1; W[1][1] = 2; W[2][2] = 4; W[3][3] = 2; W[4][4] = 1;
    W[5][5] = 2; W[6][6] = 4; W[7][7] = 8; W[8][8] = 4; W[9][9] = 2;
    W[10][10] = 4; W[11][11] = 8; W[12][12] = 16; W[13][13] = 8; W[14][14] = 4;
    W[15][15] = 2; W[16][16] = 4; W[17][17] = 8; W[18][18] = 4; W[19][19] = 2;
    W[20][20] = 1; W[21][21] = 2; W[22][22] = 4; W[23][23] = 2; W[24][24] = 1;




    for(int outloop = 0; outloop < (igMain_prev.Row()/ 5) -1; outloop++){
        window_y = 1+ outloop * 5;
        for(int innerloop = 0; innerloop < (igMain_prev.Col()/5) -1 ; innerloop++){
            window_x = 1+ innerloop * 5;


            KMatrix A(25,2);
            KMatrix b(25,1);
            KMatrix d(2,1);


            for(int i = 0; i< 5 ; i++){
                for(int j = 0; j< 5 ; j++){

                    A[i*5 + j][0] = (double)Intensity_X[window_y +i][window_x +j];
                    A[i*5 + j][1] = (double)Intensity_Y[window_y +i][window_x +j];
                    b[i*5 + j][0] = (double)Intensity_T[window_y +i][window_x +j];
                }
            }


            int test_data = b[13][0];






            KMatrix A_T = A.Tr();
            KMatrix AtxWxA = A_T*W* A;
            double determinant = (AtxWxA).Det();
            if(determinant){
                KMatrix inv = ~AtxWxA;

                d = inv*A_T*W*b;

                double test_d  = d[0][0];








                q_pForm_result->DrawLine(window_x + 2 , window_y + 2,  window_x + 2+d[0][0], window_y + 2 +d[1][0], Qt::green, 1);
            }
        }
    }



    _plpImageForm->Add(q_pForm_result);
    q_pForm_result->show();

    //UI 활성화 갱신
    UpdateUI();



}
#define Composite   0
#define New         1
typedef struct tagBlending_INFO
{
    int flag; //0: overapped , 1:Composite , 2: New
    int DtoComposite = 0;   //Distance from not overapped composite image
    int DtoNew=0;           //Distance from not overapped New image
    char Boundaryinfo =0;   //it this value is 1, that point is boundary point


}BLENDING_INFO;
void MainFrame::on_Btn_ImgBlending_clicked()
{
    //포커스 된 ImageForm으로부터 영상을 가져옴
    KImageGray   igMain;        KImageColor   icMain;
    KImageColor   icNew;
    KImageColor   icComposite;
    KImageGray   igComposite_T;
    KImageGray   igResult;      KImageColor   icResult;
    KImageGray   igMain_prev;
    ImageForm*  q_pForm_result;
    ImageForm*  q_pForm;
    KImageColor icImage1, icImage2;
    int nNumImage = 3;

    //image load
    q_pForm = new ImageForm("C:\\Homeworks_CV\\h0.bmp", "testimage0", this);

    icComposite = q_pForm->ImageColor();

    delete q_pForm;


    q_pForm = new ImageForm("C:\\Homeworks_CV\\h1.bmp", "testimage1", this);

    icImage1 = q_pForm->ImageColor();

    delete q_pForm;

    q_pForm = new ImageForm("C:\\Homeworks_CV\\h2.bmp", "testimage2", this);

    icImage2 = q_pForm->ImageColor();

    KMatrix mProject;
    //mtx file load
    KMatrix mTmp0;
    FILE *fp0 = fopen("C:\\Homeworks_CV\\h0.mtx","rb");
    bool bRead0 = mTmp0.Read(fp0);

    KMatrix mInv0;
    mInv0 = ~mTmp0;

    igComposite_T = icComposite.ColorToGray().Transformed(mInv0);
    //plgtotal
    QPolygonF plgNew, plgTotal,plgView,plgComposite,plgOverap;


    plgView <<QPoint(0,0);
    plgView <<QPoint(0,icComposite.Row()-1);
    plgView <<QPoint(icComposite.Col()-1,icComposite.Row()-1);
    plgView <<QPoint(icComposite.Col()-1,0);
    plgView <<QPoint(0,0);
    //image0(icComposite add)
    plgNew.clear();
    mProject = mInv0 * (KVector(0.0,0.0,1.0)|KVector(icComposite.Col()-1.0,0.0,1.0)|KVector(icComposite.Col()-1.0,icComposite.Row()-1.0,1.0)|KVector(0.0,icComposite.Row()-1.0,1.0)|KVector(0.0,0.0,1.0));
    for(int k = 0; k<mProject.Col();k++){
        plgNew<<QPointF(mProject[0][k]/mProject[2][k],mProject[1][k]/mProject[2][k]);
        //plgView<<QPointF(mProject[0][k]/mProject[2][k],mProject[1][k]/mProject[2][k]);

    }
    plgTotal = plgTotal.united(plgNew);

    KMatrix mTmp1;
    FILE *fp1 = fopen("C:\\Homeworks_CV\\h1.mtx","rb");
    bool bRead1 = mTmp1.Read(fp1);

    KMatrix mInv1;
    mInv1 = ~mTmp1;

    plgNew.clear();
    mProject = mInv1 * (KVector(0.0,0.0,1.0)|KVector(icImage1.Col()-1.0,0.0,1.0)|KVector(icImage1.Col()-1.0,icImage1.Row()-1.0,1.0)|KVector(0.0,icImage1.Row()-1.0,1.0)|KVector(0.0,0.0,1.0));

    for(int k = 0; k<mProject.Col();k++){
        plgNew<<QPointF(mProject[0][k]/mProject[2][k],mProject[1][k]/mProject[2][k]);

    }
    plgTotal = plgTotal.united(plgNew);

    KMatrix mTmp2;
    FILE *fp2 = fopen("C:\\Homeworks_CV\\h2.mtx","rb");
    bool bRead2 = mTmp2.Read(fp2);

    KMatrix mInv2;
    mInv2 = ~mTmp2;

    plgNew.clear();
    mProject = mInv2 * (KVector(0.0,0.0,1.0)|KVector(icImage2.Col()-1.0,0.0,1.0)|KVector(icImage2.Col()-1.0,icImage2.Row()-1.0,1.0)|KVector(0.0,icImage2.Row()-1.0,1.0)|KVector(0.0,0.0,1.0));

    for(int k = 0; k<mProject.Col();k++){
        plgNew<<QPointF(mProject[0][k]/mProject[2][k],mProject[1][k]/mProject[2][k]);

    }
    plgTotal = plgTotal.united(plgNew);


    QRect rcWin = plgTotal.boundingRect().toRect();
    double dScale = _MAX((double)(rcWin.height())/(double)(icComposite.Row()),
                         (double)(rcWin.width())/(double)(icComposite.Col()));


    mProject = mInv0 * (KVector(0.0,0.0,1.0)|KVector(icComposite.Col()-1.0,0.0,1.0)|KVector(icComposite.Col()-1.0,icComposite.Row()-1.0,1.0)|KVector(0.0,icComposite.Row()-1.0,1.0)|KVector(0.0,0.0,1.0));
    plgNew.clear();

    for(int k = 0; k<mProject.Col(); k++){
        QPointF q_ptImg(mProject[0][k]/mProject[2][k], mProject[1][k]/mProject[2][k]);

        //q_ptImg.rx() = (q_ptImg.rx() - rcWin.left())/dScale;
        //q_ptImg.ry() = (q_ptImg.ry() - rcWin.top())/dScale;
        plgComposite<<q_ptImg;

    }

    mProject = mInv1 * (KVector(0.0,0.0,1.0)|KVector(icImage1.Col()-1.0,0.0,1.0)|KVector(icImage1.Col()-1.0,icImage1.Row()-1.0,1.0)|KVector(0.0,icImage1.Row()-1.0,1.0)|KVector(0.0,0.0,1.0));
    plgNew.clear();

    for(int k = 0; k<mProject.Col(); k++){
        QPointF q_ptImg(mProject[0][k]/mProject[2][k], mProject[1][k]/mProject[2][k]);

        //q_ptImg.rx() = (q_ptImg.rx() - rcWin.left())/dScale;
        //q_ptImg.ry() = (q_ptImg.ry() - rcWin.top())/dScale;
        plgNew<<q_ptImg;

    }

    plgNew = plgNew.intersected(plgView);
    plgOverap = plgComposite.intersected(plgNew);
    QPolygonF plgOverap1 = plgOverap;
    plgComposite.united(plgNew);

    KArray<BLENDING_INFO> brushfire;
    brushfire.Create(icComposite.Row(),icComposite.Col());
    for(int i = 0; i<brushfire.Row() ; i++){
        for(int j = 0; j<brushfire.Col() ; j++){
            if(plgOverap1.containsPoint(QPointF(j,i),Qt::OddEvenFill) && plgComposite.containsPoint(QPointF(j,i),Qt::OddEvenFill)){
                brushfire[i][j].flag = 0; //white area

            }else if(plgComposite.containsPoint(QPointF(j,i),Qt::OddEvenFill) && !plgOverap1.containsPoint(QPointF(j,i),Qt::OddEvenFill)){
                brushfire[i][j].flag = 1; //composite
            }else{
                brushfire[i][j].flag = 2; //New
            }
        }
    }
    QStack<QPoint> stack_Boundary;
    for(int i = 0; i<brushfire.Row() ; i++){
        for(int j = 0; j<brushfire.Col() ; j++){
            if(brushfire[i][j].flag ==0){
                if(brushfire[i][j-1].flag || brushfire[i-1][j].flag || brushfire[i][j+1].flag || brushfire[i+1][j].flag){

                    brushfire[i][j].Boundaryinfo = 1;

                    stack_Boundary.push_back(QPoint(j,i));
                }

            }
        }
    }
    cout << stack_Boundary.count() <<endl;
    QStack<QPoint> grassfire_composite;
    QStack<QPoint> grassfire_New;
    while(!stack_Boundary.empty()){
        int x = stack_Boundary.top().x();
        int y = stack_Boundary.top().y();

        stack_Boundary.pop();
        if((x == 0) ||!plgComposite.containsPoint(QPointF(x-2,y-2),Qt::OddEvenFill)||!plgComposite.containsPoint(QPointF(x+2,y+2),Qt::OddEvenFill)){
            if(brushfire[y+1][x].flag == 0){
                brushfire[y+1][x].DtoNew = 1;
                grassfire_New.push_back(QPoint(x,y+1));
            }
            if(brushfire[y-1][x].flag == 0){
                brushfire[y-1][x].DtoNew = 1;
                grassfire_New.push_back(QPoint(x,y-1));
            }
            if(brushfire[y][x+1].flag == 0){
                brushfire[y][x+1].DtoNew = 1;
                grassfire_New.push_back(QPoint(x+1,y));
            }
            if(brushfire[y][x-1].flag == 0){
                brushfire[y][x-1].DtoNew = 1;
                grassfire_New.push_back(QPoint(x-1,y));
            }
            if(brushfire[y-1][x-1].flag == 0){
                brushfire[y-1][x-1].DtoNew = 1;
                grassfire_New.push_back(QPoint(x-1,y-1));
            }
            if(brushfire[y-1][x+1].flag == 0){
                brushfire[y-1][x+1].DtoNew = 1;
                grassfire_New.push_back(QPoint(x+1,y-1));
            }
            if(brushfire[y+1][x-1].flag == 0){
                brushfire[y+1][x-1].DtoNew = 1;
                grassfire_New.push_back(QPoint(x-1,y+1));
            }
            if(brushfire[y+1][x+1].flag == 0){
                brushfire[y+1][x+1].DtoNew = 1;
                grassfire_New.push_back(QPoint(x+1,y+1));
            }

            continue;
        }

        if(brushfire[y+1][x].flag == 0){
            brushfire[y+1][x].DtoComposite = 1;
            grassfire_composite.push_back(QPoint(x,y+1));
        }
        if(brushfire[y-1][x].flag == 0){
            brushfire[y-1][x].DtoComposite = 1;
            grassfire_composite.push_back(QPoint(x,y-1));
        }
        if(brushfire[y][x+1].flag == 0){
            brushfire[y][x+1].DtoComposite = 1;
            grassfire_composite.push_back(QPoint(x+1,y));
        }
        if(brushfire[y][x-1].flag == 0){
            brushfire[y][x-1].DtoComposite = 1;
            grassfire_composite.push_back(QPoint(x-1,y));
        }
        if(brushfire[y-1][x-1].flag == 0){
            brushfire[y-1][x-1].DtoComposite = 1;
            grassfire_composite.push_back(QPoint(x-1,y-1));
        }
        if(brushfire[y-1][x+1].flag == 0){
            brushfire[y-1][x+1].DtoComposite = 1;
            grassfire_composite.push_back(QPoint(x+1,y-1));
        }
        if(brushfire[y+1][x-1].flag == 0){
            brushfire[y+1][x-1].DtoComposite = 1;
            grassfire_composite.push_back(QPoint(x-1,y+1));
        }
        if(brushfire[y+1][x+1].flag == 0){
            brushfire[y+1][x+1].DtoComposite = 1;
            grassfire_composite.push_back(QPoint(x+1,y+1));
        }

    }
    int distance_count = 0;
    while(!grassfire_composite.empty()){
        int x = grassfire_composite.top().x();
        int y = grassfire_composite.top().y();

        grassfire_composite.pop();

        if(brushfire[y+1][x].flag == 0 && (brushfire[y+1][x].DtoComposite == 0)){
            brushfire[y+1][x].DtoComposite = brushfire[y][x].DtoComposite+1;
            //if(brushfire[y+1][x].DtoComposite > 250) brushfire[y+1][x].DtoComposite = 250;
            //if(brushfire[y+1][x].DtoComposite > 500;
            grassfire_composite.push_front(QPoint(x,y+1));
        }
        if(brushfire[y-1][x].flag == 0 && (brushfire[y-1][x].DtoComposite == 0)){
            brushfire[y-1][x].DtoComposite = brushfire[y][x].DtoComposite+1;
            //if(brushfire[y-1][x].DtoComposite > 250) brushfire[y-1][x].DtoComposite = 250;
            //if(brushfire[y-1][x].DtoComposite > 500) continue;

            grassfire_composite.push_front(QPoint(x,y-1));
        }
        if(brushfire[y][x+1].flag == 0 && (brushfire[y][x+1].DtoComposite == 0)){
            brushfire[y][x+1].DtoComposite =brushfire[y][x].DtoComposite+ 1;
            //if(brushfire[y][x+1].DtoComposite > 250) brushfire[y][x+1].DtoComposite = 250;
            //if(brushfire[y][x+1].DtoComposite > 500) continue;

            grassfire_composite.push_front(QPoint(x+1,y));
        }
        if(brushfire[y][x-1].flag == 0 && (brushfire[y][x-1].DtoComposite == 0)){
            brushfire[y][x-1].DtoComposite = brushfire[y][x].DtoComposite+1;
            //if(brushfire[y][x-1].DtoComposite > 250) brushfire[y][x-1].DtoComposite = 250;
            //            if(brushfire[y][x-1].DtoComposite > 500) continue;

            grassfire_composite.push_front(QPoint(x-1,y));
        }
        if(brushfire[y-1][x-1].flag == 0 && (brushfire[y-1][x-1].DtoComposite == 0)){
            brushfire[y-1][x-1].DtoComposite = brushfire[y][x].DtoComposite+1;
            //if(brushfire[y-1][x-1].DtoComposite > 250) brushfire[y-1][x-1].DtoComposite = 250;
            //            if(brushfire[y-1][x-1].DtoComposite > 500) continue;

            grassfire_composite.push_front(QPoint(x-1,y-1));
        }
        if(brushfire[y-1][x+1].flag == 0 && (brushfire[y-1][x+1].DtoComposite == 0)){
            brushfire[y-1][x+1].DtoComposite = brushfire[y][x].DtoComposite+1;
            //if(brushfire[y-1][x+1].DtoComposite > 250) brushfire[y-1][x+1].DtoComposite = 250;
            //            if(brushfire[y-1][x+1].DtoComposite > 500) continue;
            grassfire_composite.push_front(QPoint(x+1,y-1));
        }
        if(brushfire[y+1][x-1].flag == 0 && (brushfire[y+1][x-1].DtoComposite == 0)){
            brushfire[y+1][x-1].DtoComposite = brushfire[y][x].DtoComposite+1;
            //if(brushfire[y+1][x-1].DtoComposite > 250) brushfire[y+1][x-1].DtoComposite = 250;
            //            if(brushfire[y+1][x-1].DtoComposite > 500) continue;
            grassfire_composite.push_front(QPoint(x-1,y+1));
        }
        if(brushfire[y+1][x+1].flag == 0 && (brushfire[y+1][x+1].DtoComposite == 0)){
            brushfire[y+1][x+1].DtoComposite = brushfire[y][x].DtoComposite+1;
            //if(brushfire[y+1][x+1].DtoComposite > 250) brushfire[y+1][x+1].DtoComposite = 250;
            //            if(brushfire[y+1][x+1].DtoComposite > 500) continue;
            grassfire_composite.push_front(QPoint(x+1,y+1));
        }




    }
    while(!grassfire_New.empty()){
        int x = grassfire_New.top().x();
        int y = grassfire_New.top().y();

        grassfire_New.pop();

        if(brushfire[y+1][x].flag == 0 && (brushfire[y+1][x].DtoNew == 0)){
            brushfire[y+1][x].DtoNew = brushfire[y][x].DtoNew+1;
            //if(brushfire[y+1][x].DtoNew > 250) brushfire[y+1][x].DtoNew = 250;
            //if(brushfire[y+1][x].DtoNew > 500;
            grassfire_New.push_front(QPoint(x,y+1));
        }
        if(brushfire[y-1][x].flag == 0 && (brushfire[y-1][x].DtoNew == 0)){
            brushfire[y-1][x].DtoNew = brushfire[y][x].DtoNew+1;
            //if(brushfire[y-1][x].DtoNew > 250) brushfire[y-1][x].DtoNew = 250;
            //if(brushfire[y-1][x].DtoNew > 500) continue;

            grassfire_New.push_front(QPoint(x,y-1));
        }
        if(brushfire[y][x+1].flag == 0 && (brushfire[y][x+1].DtoNew == 0)){
            brushfire[y][x+1].DtoNew =brushfire[y][x].DtoNew+ 1;
            //if(brushfire[y][x+1].DtoNew > 250) brushfire[y][x+1].DtoNew = 250;
            //if(brushfire[y][x+1].DtoNew > 500) continue;

            grassfire_New.push_front(QPoint(x+1,y));
        }
        if(brushfire[y][x-1].flag == 0 && (brushfire[y][x-1].DtoNew == 0)){
            brushfire[y][x-1].DtoNew = brushfire[y][x].DtoNew+1;
            //if(brushfire[y][x-1].DtoNew > 250) brushfire[y][x-1].DtoNew = 250;
            //            if(brushfire[y][x-1].DtoNew > 500) continue;

            grassfire_New.push_front(QPoint(x-1,y));
        }
        if(brushfire[y-1][x-1].flag == 0 && (brushfire[y-1][x-1].DtoNew == 0)){
            brushfire[y-1][x-1].DtoNew = brushfire[y][x].DtoNew+1;
            //if(brushfire[y-1][x-1].DtoNew > 250) brushfire[y-1][x-1].DtoNew = 250;
            //            if(brushfire[y-1][x-1].DtoNew > 500) continue;

            grassfire_New.push_front(QPoint(x-1,y-1));
        }
        if(brushfire[y-1][x+1].flag == 0 && (brushfire[y-1][x+1].DtoNew == 0)){
            brushfire[y-1][x+1].DtoNew = brushfire[y][x].DtoNew+1;
            //if(brushfire[y-1][x+1].DtoNew > 250) brushfire[y-1][x+1].DtoNew = 250;
            //            if(brushfire[y-1][x+1].DtoNew > 500) continue;
            grassfire_New.push_front(QPoint(x+1,y-1));
        }
        if(brushfire[y+1][x-1].flag == 0 && (brushfire[y+1][x-1].DtoNew == 0)){
            brushfire[y+1][x-1].DtoNew = brushfire[y][x].DtoNew+1;
            //if(brushfire[y+1][x-1].DtoNew > 250) brushfire[y+1][x-1].DtoNew = 250;
            //            if(brushfire[y+1][x-1].DtoNew > 500) continue;
            grassfire_New.push_front(QPoint(x-1,y+1));
        }
        if(brushfire[y+1][x+1].flag == 0 && (brushfire[y+1][x+1].DtoNew == 0)){
            brushfire[y+1][x+1].DtoNew = brushfire[y][x].DtoNew+1;
            //if(brushfire[y+1][x+1].DtoNew > 250) brushfire[y+1][x+1].DtoNew = 250;
            //            if(brushfire[y+1][x+1].DtoNew > 500) continue;
            grassfire_New.push_front(QPoint(x+1,y+1));
        }

    }


    igMain = icComposite.ColorToGray();
    igMain = igMain.Transform(mInv0);

    KImageGray igImage1;
    igImage1 = icImage1.ColorToGray();
    igImage1 = igImage1.Transform(mInv1);

    for(int i = 0; i<brushfire.Row() ; i++){
        for(int j = 0; j<brushfire.Col(); j++){
            if(brushfire[i][j].flag == 0){
                int DtoComposite = brushfire[i][j].DtoComposite;
                int DtoNew = brushfire[i][j].DtoNew;
                igMain._ppA[i][j] = ((igMain._ppA[i][j])*DtoNew + (igImage1._ppA[i][j])*DtoComposite)/(DtoComposite+DtoNew);
            }
        }
    }
    plgComposite = plgComposite.united(plgOverap1);

    mProject = mInv2 * (KVector(0.0,0.0,1.0)|KVector(icImage2.Col()-1.0,0.0,1.0)|KVector(icImage2.Col()-1.0,icImage2.Row()-1.0,1.0)|KVector(0.0,icImage2.Row()-1.0,1.0)|KVector(0.0,0.0,1.0));
    plgNew.clear();

    for(int k = 0; k<mProject.Col(); k++){
        QPointF q_ptImg(mProject[0][k]/mProject[2][k], mProject[1][k]/mProject[2][k]);

        //q_ptImg.rx() = (q_ptImg.rx() - rcWin.left())/dScale;
        //q_ptImg.ry() = (q_ptImg.ry() - rcWin.top())/dScale;
        plgNew<<q_ptImg;

    }
    plgNew = plgNew.intersected(plgView);
    plgOverap = plgComposite.intersected(plgNew);
    // here fill code about blending
    KImageGray igImage2;
    igImage2 = icImage2.ColorToGray();
    igImage2 = igImage2.Transform(mInv2);

    KArray<BLENDING_INFO> brushfire1;
    brushfire1.Create(icComposite.Row(),igImage2.Col());
    for(int i = 0; i<brushfire1.Row() ; i++){
        for(int j = 0; j<brushfire1.Col() ; j++){
            if(plgOverap.containsPoint(QPointF(j,i),Qt::OddEvenFill) && plgComposite.containsPoint(QPointF(j,i),Qt::OddEvenFill)){
                brushfire1[i][j].flag = 0; //white area

            }else if(plgComposite.containsPoint(QPointF(j,i),Qt::OddEvenFill) && !plgOverap.containsPoint(QPointF(j,i),Qt::OddEvenFill)){
                brushfire1[i][j].flag = 1; //composite
            }else{
                brushfire1[i][j].flag = 2; //New
            }
        }
    }
    stack_Boundary.clear();
    for(int i = 0; i<brushfire1.Row() ; i++){
        for(int j = 0; j<brushfire1.Col() ; j++){
            if(brushfire1[i][j].flag ==0){
                if(brushfire1[i][j-1].flag || brushfire1[i-1][j].flag || brushfire1[i][j+1].flag || brushfire1[i+1][j].flag){

                    brushfire1[i][j].Boundaryinfo = 1;

                    stack_Boundary.push_back(QPoint(j,i));
                }

            }
        }
    }
    cout << stack_Boundary.count() <<endl;
    grassfire_composite.clear();
    grassfire_New.clear();
    while(!stack_Boundary.empty()){
        int x = stack_Boundary.top().x();
        int y = stack_Boundary.top().y();

        stack_Boundary.pop();
        if(!plgOverap.containsPoint(QPointF(x+1,y),Qt::OddEvenFill)){
            if(brushfire1[y+1][x].flag == 0){
                brushfire1[y+1][x].DtoNew = 1;
                grassfire_New.push_back(QPoint(x,y+1));
            }
            if(brushfire1[y-1][x].flag == 0){
                brushfire1[y-1][x].DtoNew = 1;
                grassfire_New.push_back(QPoint(x,y-1));
            }
            if(brushfire1[y][x+1].flag == 0){
                brushfire1[y][x+1].DtoNew = 1;
                grassfire_New.push_back(QPoint(x+1,y));
            }
            if(brushfire1[y][x-1].flag == 0){
                brushfire1[y][x-1].DtoNew = 1;
                grassfire_New.push_back(QPoint(x-1,y));
            }
            if(brushfire1[y-1][x-1].flag == 0){
                brushfire1[y-1][x-1].DtoNew = 1;
                grassfire_New.push_back(QPoint(x-1,y-1));
            }
            if(brushfire1[y-1][x+1].flag == 0){
                brushfire1[y-1][x+1].DtoNew = 1;
                grassfire_New.push_back(QPoint(x+1,y-1));
            }
            if(brushfire1[y+1][x-1].flag == 0){
                brushfire1[y+1][x-1].DtoNew = 1;
                grassfire_New.push_back(QPoint(x-1,y+1));
            }
            if(brushfire1[y+1][x+1].flag == 0){
                brushfire1[y+1][x+1].DtoNew = 1;
                grassfire_New.push_back(QPoint(x+1,y+1));
            }

            continue;
        }

        if(brushfire1[y+1][x].flag == 0){
            brushfire1[y+1][x].DtoComposite = 1;
            grassfire_composite.push_back(QPoint(x,y+1));
        }
        if(brushfire1[y-1][x].flag == 0){
            brushfire1[y-1][x].DtoComposite = 1;
            grassfire_composite.push_back(QPoint(x,y-1));
        }
        if(brushfire1[y][x+1].flag == 0){
            brushfire1[y][x+1].DtoComposite = 1;
            grassfire_composite.push_back(QPoint(x+1,y));
        }
        if(brushfire1[y][x-1].flag == 0){
            brushfire1[y][x-1].DtoComposite = 1;
            grassfire_composite.push_back(QPoint(x-1,y));
        }
        if(brushfire1[y-1][x-1].flag == 0){
            brushfire1[y-1][x-1].DtoComposite = 1;
            grassfire_composite.push_back(QPoint(x-1,y-1));
        }
        if(brushfire1[y-1][x+1].flag == 0){
            brushfire1[y-1][x+1].DtoComposite = 1;
            grassfire_composite.push_back(QPoint(x+1,y-1));
        }
        if(brushfire1[y+1][x-1].flag == 0){
            brushfire1[y+1][x-1].DtoComposite = 1;
            grassfire_composite.push_back(QPoint(x-1,y+1));
        }
        if(brushfire1[y+1][x+1].flag == 0){
            brushfire1[y+1][x+1].DtoComposite = 1;
            grassfire_composite.push_back(QPoint(x+1,y+1));
        }

    }

    while(!grassfire_composite.empty()){
        int x = grassfire_composite.top().x();
        int y = grassfire_composite.top().y();

        grassfire_composite.pop();

        if(brushfire1[y+1][x].flag == 0 && (brushfire1[y+1][x].DtoComposite == 0)){
            brushfire1[y+1][x].DtoComposite = brushfire1[y][x].DtoComposite+1;
            //if(brushfire[y+1][x].DtoComposite > 250) brushfire[y+1][x].DtoComposite = 250;
            //if(brushfire[y+1][x].DtoComposite > 500;
            grassfire_composite.push_front(QPoint(x,y+1));
        }
        if(brushfire1[y-1][x].flag == 0 && (brushfire1[y-1][x].DtoComposite == 0)){
            brushfire1[y-1][x].DtoComposite = brushfire1[y][x].DtoComposite+1;
            //if(brushfire[y-1][x].DtoComposite > 250) brushfire[y-1][x].DtoComposite = 250;
            //if(brushfire[y-1][x].DtoComposite > 500) continue;

            grassfire_composite.push_front(QPoint(x,y-1));
        }
        if(brushfire1[y][x+1].flag == 0 && (brushfire1[y][x+1].DtoComposite == 0)){
            brushfire1[y][x+1].DtoComposite =brushfire1[y][x].DtoComposite+ 1;
            //if(brushfire[y][x+1].DtoComposite > 250) brushfire[y][x+1].DtoComposite = 250;
            //if(brushfire[y][x+1].DtoComposite > 500) continue;

            grassfire_composite.push_front(QPoint(x+1,y));
        }
        if(brushfire1[y][x-1].flag == 0 && (brushfire1[y][x-1].DtoComposite == 0)){
            brushfire1[y][x-1].DtoComposite = brushfire1[y][x].DtoComposite+1;
            //if(brushfire[y][x-1].DtoComposite > 250) brushfire[y][x-1].DtoComposite = 250;
            //            if(brushfire[y][x-1].DtoComposite > 500) continue;

            grassfire_composite.push_front(QPoint(x-1,y));
        }
        if(brushfire1[y-1][x-1].flag == 0 && (brushfire1[y-1][x-1].DtoComposite == 0)){
            brushfire1[y-1][x-1].DtoComposite = brushfire1[y][x].DtoComposite+1;
            //if(brushfire[y-1][x-1].DtoComposite > 250) brushfire[y-1][x-1].DtoComposite = 250;
            //            if(brushfire[y-1][x-1].DtoComposite > 500) continue;

            grassfire_composite.push_front(QPoint(x-1,y-1));
        }
        if(brushfire1[y-1][x+1].flag == 0 && (brushfire1[y-1][x+1].DtoComposite == 0)){
            brushfire1[y-1][x+1].DtoComposite = brushfire1[y][x].DtoComposite+1;
            //if(brushfire[y-1][x+1].DtoComposite > 250) brushfire[y-1][x+1].DtoComposite = 250;
            //            if(brushfire[y-1][x+1].DtoComposite > 500) continue;
            grassfire_composite.push_front(QPoint(x+1,y-1));
        }
        if(brushfire1[y+1][x-1].flag == 0 && (brushfire1[y+1][x-1].DtoComposite == 0)){
            brushfire1[y+1][x-1].DtoComposite = brushfire1[y][x].DtoComposite+1;
            //if(brushfire[y+1][x-1].DtoComposite > 250) brushfire[y+1][x-1].DtoComposite = 250;
            //            if(brushfire[y+1][x-1].DtoComposite > 500) continue;
            grassfire_composite.push_front(QPoint(x-1,y+1));
        }
        if(brushfire1[y+1][x+1].flag == 0 && (brushfire1[y+1][x+1].DtoComposite == 0)){
            brushfire1[y+1][x+1].DtoComposite = brushfire1[y][x].DtoComposite+1;
            //if(brushfire[y+1][x+1].DtoComposite > 250) brushfire[y+1][x+1].DtoComposite = 250;
            //            if(brushfire[y+1][x+1].DtoComposite > 500) continue;
            grassfire_composite.push_front(QPoint(x+1,y+1));
        }




    }
    while(!grassfire_New.empty()){
        int x = grassfire_New.top().x();
        int y = grassfire_New.top().y();

        grassfire_New.pop();

        if(brushfire1[y+1][x].flag == 0 && (brushfire1[y+1][x].DtoNew == 0)){
            brushfire1[y+1][x].DtoNew = brushfire1[y][x].DtoNew+1;
            //if(brushfire[y+1][x].DtoNew > 250) brushfire[y+1][x].DtoNew = 250;
            //if(brushfire[y+1][x].DtoNew > 500;
            grassfire_New.push_front(QPoint(x,y+1));
        }
        if(brushfire1[y-1][x].flag == 0 && (brushfire1[y-1][x].DtoNew == 0)){
            brushfire1[y-1][x].DtoNew = brushfire1[y][x].DtoNew+1;
            //if(brushfire[y-1][x].DtoNew > 250) brushfire[y-1][x].DtoNew = 250;
            //if(brushfire[y-1][x].DtoNew > 500) continue;

            grassfire_New.push_front(QPoint(x,y-1));
        }
        if(brushfire1[y][x+1].flag == 0 && (brushfire1[y][x+1].DtoNew == 0)){
            brushfire1[y][x+1].DtoNew =brushfire1[y][x].DtoNew+ 1;
            //if(brushfire[y][x+1].DtoNew > 250) brushfire[y][x+1].DtoNew = 250;
            //if(brushfire[y][x+1].DtoNew > 500) continue;

            grassfire_New.push_front(QPoint(x+1,y));
        }
        if(brushfire1[y][x-1].flag == 0 && (brushfire1[y][x-1].DtoNew == 0)){
            brushfire1[y][x-1].DtoNew = brushfire1[y][x].DtoNew+1;
            //if(brushfire[y][x-1].DtoNew > 250) brushfire[y][x-1].DtoNew = 250;
            //            if(brushfire[y][x-1].DtoNew > 500) continue;

            grassfire_New.push_front(QPoint(x-1,y));
        }
        if(brushfire1[y-1][x-1].flag == 0 && (brushfire1[y-1][x-1].DtoNew == 0)){
            brushfire1[y-1][x-1].DtoNew = brushfire1[y][x].DtoNew+1;
            //if(brushfire[y-1][x-1].DtoNew > 250) brushfire[y-1][x-1].DtoNew = 250;
            //            if(brushfire[y-1][x-1].DtoNew > 500) continue;

            grassfire_New.push_front(QPoint(x-1,y-1));
        }
        if(brushfire1[y-1][x+1].flag == 0 && (brushfire1[y-1][x+1].DtoNew == 0)){
            brushfire1[y-1][x+1].DtoNew = brushfire1[y][x].DtoNew+1;
            //if(brushfire[y-1][x+1].DtoNew > 250) brushfire[y-1][x+1].DtoNew = 250;
            //            if(brushfire[y-1][x+1].DtoNew > 500) continue;
            grassfire_New.push_front(QPoint(x+1,y-1));
        }
        if(brushfire1[y+1][x-1].flag == 0 && (brushfire1[y+1][x-1].DtoNew == 0)){
            brushfire1[y+1][x-1].DtoNew = brushfire1[y][x].DtoNew+1;
            //if(brushfire[y+1][x-1].DtoNew > 250) brushfire[y+1][x-1].DtoNew = 250;
            //            if(brushfire[y+1][x-1].DtoNew > 500) continue;
            grassfire_New.push_front(QPoint(x-1,y+1));
        }
        if(brushfire1[y+1][x+1].flag == 0 && (brushfire1[y+1][x+1].DtoNew == 0)){
            brushfire1[y+1][x+1].DtoNew = brushfire1[y][x].DtoNew+1;
            //if(brushfire[y+1][x+1].DtoNew > 250) brushfire[y+1][x+1].DtoNew = 250;
            //            if(brushfire[y+1][x+1].DtoNew > 500) continue;
            grassfire_New.push_front(QPoint(x+1,y+1));
        }
    }

    for(int i = 0; i<brushfire1.Row() ; i++){
        for(int j = 0; j<brushfire1.Col(); j++){
            if(brushfire1[i][j].flag == 0){
                int DtoComposite = brushfire1[i][j].DtoComposite;
                int DtoNew = brushfire1[i][j].DtoNew;
                igMain._ppA[i][j] = ((igMain._ppA[i][j])*DtoNew + (igImage2._ppA[i][j])*DtoComposite)/(DtoComposite+DtoNew);
            }else if(brushfire1[i][j].flag == 2){
                igMain._ppA[i][j] = igImage2._ppA[i][j];
            }
        }
    }
    q_pForm_result = new ImageForm(igMain, "test", this);

    _plpImageForm->Add(q_pForm_result);
    q_pForm_result->show();

    //UI 활성화 갱신
    UpdateUI();
}
