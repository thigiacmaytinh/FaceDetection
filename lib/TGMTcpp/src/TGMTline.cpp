#include "TGMTline.h"
#include "TGMTfile.h"
#include "TGMTdebugger.h"
#include "TGMTbrightness.h"
#include "TGMTcolor.h"

TGMTline* TGMTline::instance = nullptr;

TGMTline::TGMTline()
{	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TGMTline::~TGMTline()
{
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void mergeRelatedLines(std::vector<cv::Vec2f> *lines, cv::Mat &img)
{
    std::vector<cv::Vec2f>::iterator current;
    for(current=lines->begin();current!=lines->end();current++)
    {
		if((*current)[0]==0 && (*current)[1]==-100) continue;
		float p1 = (*current)[0];
        float theta1 = (*current)[1];
		cv::Point pt1current, pt2current;
        if(theta1>CV_PI*45/180 && theta1<CV_PI*135/180)
        {
            pt1current.x=0;

            pt1current.y = p1/sin(theta1);

            pt2current.x=img.size().width;
            pt2current.y=-pt2current.x/tan(theta1) + p1/sin(theta1);
        }
        else
        {
            pt1current.y=0;

            pt1current.x=p1/cos(theta1);

            pt2current.y=img.size().height;
            pt2current.x=-pt2current.y/tan(theta1) + p1/cos(theta1);

        }
		std::vector<cv::Vec2f>::iterator pos;
        for(pos=lines->begin();pos!=lines->end();pos++)
        {
            if(*current==*pos) continue;

			if(fabs((*pos)[0]-(*current)[0])<20 && fabs((*pos)[1]-(*current)[1])<CV_PI*10/180)
            {
                float p = (*pos)[0];
                float theta = (*pos)[1];
				cv::Point pt1, pt2;
                if((*pos)[1]>CV_PI*45/180 && (*pos)[1]<CV_PI*135/180)
                {
                    pt1.x=0;
                    pt1.y = p/sin(theta);
                    pt2.x=img.size().width;
                    pt2.y=-pt2.x/tan(theta) + p/sin(theta);
                }
                else
                {
                    pt1.y=0;
                    pt1.x=p/cos(theta);
                    pt2.y=img.size().height;
                    pt2.x=-pt2.y/tan(theta) + p/cos(theta);
                }
				if(((double)(pt1.x-pt1current.x)*(pt1.x-pt1current.x) + (pt1.y-pt1current.y)*(pt1.y-pt1current.y)<64*64) &&
					((double)(pt2.x-pt2current.x)*(pt2.x-pt2current.x) + (pt2.y-pt2current.y)*(pt2.y-pt2current.y)<64*64))
                {
                    // Merge the two
                    (*current)[0] = ((*current)[0]+(*pos)[0])/2;

                    (*current)[1] = ((*current)[1]+(*pos)[1])/2;

                    (*pos)[0]=0;
                    (*pos)[1]=-100;
                }
			}
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<KeyLine> TGMTline::DetectLine(cv::Mat matInput)
{	
	cv::Mat mat = matInput.clone();
	//improve contrast
	TGMTbrightness::EqualizeHist(mat);
	TGMTbrightness::AutoContrast(mat);
	

	std::vector<KeyLine> lines;
	cv::Mat mask = cv::Mat::ones(mat.size(), CV_8UC1);
	m_binaryDescriptor->detect(mat, lines, mask);

	DEBUG_OUT("Detected %d lines", lines.size());
	return lines;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cv::Mat TGMTline::DrawLine(cv::Mat& matInput, KeyLine line, cv::Scalar color)
{
	cv::Point pt1 = cv::Point(line.startPointX, line.startPointY);
	cv::Point pt2 = cv::Point(line.endPointX, line.endPointY);

	cv::line(matInput, pt1, pt2, color != UNDEFINED_COLOR ? color : TGMTcolor::GetRandomColor(), 2);
	return matInput;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cv::Mat TGMTline::DrawLines(cv::Mat& matInput, std::vector<KeyLine> lines, cv::Scalar color)
{
	for (uint i = 0; i < lines.size(); i++)
	{
		if (lines[i].octave == 0)
		{
			DrawLine(matInput, lines[i], color);
		}
	}
	DEBUG_IMAGE(matInput, "draw");
	
	return matInput;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTline::DetectAndDrawLine(cv::Mat& matInput)
{
	DEBUG_IMAGE(matInput, "mat input");
	START_COUNT_TIME("Detect line");
	std::vector<KeyLine> lines = DetectLine(matInput);
	DrawLines(matInput, lines);
	STOP_AND_PRINT_COUNT_TIME("Detect line");

#ifdef _DEBUG
	cv::waitKey();
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cv::Point TGMTline::GetIntersectPoint(KeyLine line1, KeyLine line2)
{
	ASSERT(false, "todo");
	return cv::Point(0, 0);
}