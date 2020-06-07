/*
    Description: Hybrid Methods Generation
    Author: linlinge
    Date: 2020.04.05
*/
#pragma once
#include "Table.h"
#include "OLEModule.h"
#include "SignalProcessing.h"
#include "VectorExtend.h"
#include "StringExtend.h"

#define SLOPE_BIN   0x01
#define SLOPE_COL   0x02
#define MEVAL_BIN   0x04
#define MEVAL_COL   0x08
#define DENSITY_COL 0x10
#define STATUS      0xFF
extern pcl::PointCloud<PointType>::Ptr cloud_;
extern pcl::search::KdTree<PointType>::Ptr kdtree_;
extern OLEModule* pOle_;
extern vector<int> status_;
extern int accumulator;
extern int is_show_progress;
class HybridMethods
{
    public:
        /* Variables */
        Table<Rrd1> rst_meval_;
        Table<Rrd1> rst_slope_;
        Table<Rrd1> rst_db2_;
        Table<Rrd1> rst_density_;
        Table<Rrd1> rst_nid_;
        pcl::PointCloud<PointType>::Ptr cloud_;
        pcl::search::KdTree<PointType>::Ptr kdtree_;
        OLEModule* pOle_;
        int N_;
        int accumulator_;
        double startTime_,endTime_;
        string store_path_;

        /* Construction */
        HybridMethods(pcl::PointCloud<PointType>::Ptr cloud,pcl::search::KdTree<PointType>::Ptr kdtree,OLEModule* pOle,string store_path="Result"){
            cloud_=cloud;
            kdtree_=kdtree;
            pOle_=pOle;
            N_=cloud_->points.size();
            status_.resize(N_);
            for(int i=0;i<N_;i++){
                status_[i]=0;
            }
            accumulator_=1;
            startTime_=omp_get_wtime();
            store_path_=store_path;
        }

        /* Function Modules*/
        void FM_Prox(int K, double kIQR,string domain="0");          // Outliers detection based on proximity
        void FM_MEval(int K, double kIQR,string domain="0");      // IRPC: Irregular and Regular Points Classfication based on Minor Eigenvalue
        void FM_NID(int K, double kIQR, string domain="0");
        void FM_D4(int K,double P,string domain="0");
        void FM_Density(int K, double alpha,string domain="0");
        void FM_RegionGrowth(double thresh_eclidean, double thresh_tolerance,double thresh_kIQR, string domain="0");
        void FM_MajorityVote(int K,string domain="0");

        /* Combination Operator */
        vector<int> status_;

        /* Status Operations */
        void GetScopeIndices(string str,vector<int>& cIdx);
        void DemonstrateResult(string path,Table<Rrd1>& tb);
        void DemonstrateResult(string path);
};
