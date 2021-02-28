/*
 * Software License Agreement (BSD License)
 *
 *  Point Cloud Library (PCL) - www.pointclouds.org
 *  Copyright (c) 2010-2012, Willow Garage, Inc.
 *
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of the copyright holder(s) nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 * $Id$
 *
 */

#include <Eigen/Eigenvalues> // for SelfAdjointEigenSolver

#include <pcl/test/gtest.h>
#include <pcl/pcl_tests.h>
#include <pcl/common/common.h>
#include <pcl/common/distances.h>
#include <pcl/common/intersections.h>
#include <pcl/common/io.h>
#include <pcl/common/eigen.h>
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl/common/point_tests.h> // for isFinite

using namespace pcl;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST (PCL, PointXYZRGB)
{
  PointXYZRGB p;

  std::uint8_t r = 127, g = 64, b = 254;
  p.r = r;
  p.g = g;
  p.b = b;

  std::uint8_t rr = (p.rgba >> 16) & 0x0000ff;
  std::uint8_t gg = (p.rgba >> 8)  & 0x0000ff;
  std::uint8_t bb = (p.rgba)       & 0x0000ff;

  EXPECT_EQ (r, rr);
  EXPECT_EQ (g, gg);
  EXPECT_EQ (b, bb);
  EXPECT_EQ (rr, 127);
  EXPECT_EQ (gg, 64);
  EXPECT_EQ (bb, 254);

  p.r = 0; p.g = 127; p.b = 0;
  std::uint32_t rgb = p.rgba;
  rr = (rgb >> 16) & 0x0000ff;
  gg = (rgb >> 8)  & 0x0000ff;
  bb = (rgb)       & 0x0000ff;

  EXPECT_EQ (rr, 0);
  EXPECT_EQ (gg, 127);
  EXPECT_EQ (bb, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST (PCL, PointXYZRGBNormal)
{
  PointXYZRGBNormal p;

  std::uint8_t r = 127, g = 64, b = 254;
  std::uint32_t rgb = (static_cast<std::uint32_t> (r) << 16 |
                  static_cast<std::uint32_t> (g) << 8 |
                  static_cast<std::uint32_t> (b));
  p.rgba = rgb;

  std::uint8_t rr = (p.rgba >> 16) & 0x0000ff;
  std::uint8_t gg = (p.rgba >> 8)  & 0x0000ff;
  std::uint8_t bb = (p.rgba)       & 0x0000ff;

  EXPECT_EQ (r, rr);
  EXPECT_EQ (g, gg);
  EXPECT_EQ (b, bb);
  EXPECT_EQ (rr, 127);
  EXPECT_EQ (gg, 64);
  EXPECT_EQ (bb, 254);

  p.r = 0; p.g = 127; p.b = 0;
  rgb = p.rgba;
  rr = (rgb >> 16) & 0x0000ff;
  gg = (rgb >> 8)  & 0x0000ff;
  bb = (rgb)       & 0x0000ff;

  EXPECT_EQ (rr, 0);
  EXPECT_EQ (gg, 127);
  EXPECT_EQ (bb, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(PCL, isFinite)
{
  PointXYZ p;
  p.x = std::numeric_limits<float>::quiet_NaN ();
  EXPECT_FALSE (isFinite (p));
  Normal n;
  n.normal_x = std::numeric_limits<float>::quiet_NaN ();
  EXPECT_FALSE (isFinite (n));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST (PCL, Common)
{
  PointXYZ p1, p2, p3;
  p1.x = 1; p1.y = p1.z = 0;
  p2.y = 1; p2.x = p2.z = 0;
  p3.z = 1; p3.x = p3.y = 0;
  double radius = getCircumcircleRadius (p1, p2, p3);
  EXPECT_NEAR (radius, 0.816497, 1e-4);

  Eigen::Vector4f pt (1,0,0,0), line_pt (0,0,0,0), line_dir (1,1,0,0);
  double point2line_disance = sqrt (sqrPointToLineDistance (pt, line_pt, line_dir));
  EXPECT_NEAR (point2line_disance, sqrt(2.0)/2, 1e-4);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST (PCL, Eigen)
{
  Eigen::Matrix3f mat, vec;
  mat << 0.000536227f, -1.56178e-05f, -9.47391e-05f, -1.56178e-05f, 0.000297322f, -0.000148785f, -9.47391e-05f, -0.000148785f, 9.7827e-05f;
  Eigen::Vector3f val;

  eigen33 (mat, vec, val);

  EXPECT_NEAR (std::abs (vec (0, 0)), 0.168841, 1e-4); EXPECT_NEAR (std::abs (vec (0, 1)), 0.161623, 1e-4); EXPECT_NEAR (std::abs (vec (0, 2)), 0.972302, 1e-4);
  EXPECT_NEAR (std::abs (vec (1, 0)), 0.451632, 1e-4); EXPECT_NEAR (std::abs (vec (1, 1)), 0.889498, 1e-4); EXPECT_NEAR (std::abs (vec (1, 2)), 0.0694328, 1e-4);
  EXPECT_NEAR (std::abs (vec (2, 0)), 0.876082, 1e-4); EXPECT_NEAR (std::abs (vec (2, 1)), 0.4274,   1e-4); EXPECT_NEAR (std::abs (vec (2, 2)), 0.223178, 1e-4);

  EXPECT_NEAR (val (0), 2.86806e-06, 1e-4); EXPECT_NEAR (val (1), 0.00037165, 1e-4); EXPECT_NEAR (val (2), 0.000556858, 1e-4);

  Eigen::SelfAdjointEigenSolver<Eigen::Matrix3f> eig (mat);

  EXPECT_NEAR (eig.eigenvectors () (0, 0), -0.168841, 1e-4); EXPECT_NEAR (eig.eigenvectors () (0, 1),  0.161623, 1e-4); EXPECT_NEAR (eig.eigenvectors () (0, 2),  0.972302, 1e-4);
  EXPECT_NEAR (eig.eigenvectors () (1, 0), -0.451632, 1e-4); EXPECT_NEAR (eig.eigenvectors () (1, 1), -0.889498, 1e-4); EXPECT_NEAR (eig.eigenvectors () (1, 2),  0.0694328, 1e-4);
  EXPECT_NEAR (eig.eigenvectors () (2, 0), -0.876083, 1e-4); EXPECT_NEAR (eig.eigenvectors () (2, 1),  0.4274,   1e-4); EXPECT_NEAR (eig.eigenvectors () (2, 2), -0.223178, 1e-4);

  EXPECT_NEAR (eig.eigenvalues () (0), 2.86806e-06, 1e-4); EXPECT_NEAR (eig.eigenvalues () (1), 0.00037165, 1e-4); EXPECT_NEAR (eig.eigenvalues () (2), 0.000556858, 1e-4);

  Eigen::Vector3f eivals = mat.selfadjointView<Eigen::Lower>().eigenvalues ();

  EXPECT_NEAR (eivals (0), 2.86806e-06, 1e-4); EXPECT_NEAR (eivals (1), 0.00037165, 1e-4); EXPECT_NEAR (eivals (2), 0.000556858, 1e-4);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct pointCloudTest : public testing::Test {
  protected:
    virtual void SetUp() {
      cloud.width = 640;
      cloud.height = 480;
    }

    // add Teardown() if needed.

    void set_cloud(size_t size, const PointXYZ& point, int32_t width) {
      std::vector<PointXYZ> pointVec;
      pointVec.resize (size, point);
      cloud.assign (pointVec.begin(), pointVec.end(), width);
      EXPECT_TRUE (cloud.isOrganized ());
    }

    PointCloud<PointXYZ> cloud;
};

TEST_F (pointCloudTest, pointCloud_is_organized)
{
  EXPECT_TRUE (cloud.isOrganized ());
}

TEST_F (pointCloudTest, pointCloud_not_organized)
{
  cloud.height = 1;
  EXPECT_FALSE (cloud.isOrganized ());
}

TEST_F (pointCloudTest, pointCloud_getMatrixXfMap)
{
  cloud.width = 10;
  for (std::uint32_t i = 0; i < cloud.width*cloud.height; ++i)
  {
    float j = static_cast<float> (i);
    cloud.emplace_back(3.0f * j + 0.0f, 3.0f * j + 1.0f, 3.0f * j + 2.0f);
  }

  Eigen::MatrixXf mat_xyz1 = cloud.getMatrixXfMap ();
  Eigen::MatrixXf mat_xyz = cloud.getMatrixXfMap (3, 4, 0);

  if (Eigen::MatrixXf::Flags & Eigen::RowMajorBit)
  {
    EXPECT_EQ (mat_xyz1.cols (), 4);
    EXPECT_EQ (mat_xyz1.rows (), cloud.width);
    EXPECT_EQ (mat_xyz1 (0, 0), 0);
    EXPECT_EQ (mat_xyz1 (cloud.width - 1, 2), 3 * cloud.width - 1);   // = 29

    EXPECT_EQ (mat_xyz.cols (), 3);
    EXPECT_EQ (mat_xyz.rows (), cloud.width);
    EXPECT_EQ (mat_xyz (0, 0), 0);
    EXPECT_EQ (mat_xyz (cloud.width - 1, 2), 3 * cloud.width - 1);    // = 29
  }
  else
  {
    EXPECT_EQ (mat_xyz1.cols (), cloud.width);
    EXPECT_EQ (mat_xyz1.rows (), 4);
    EXPECT_EQ (mat_xyz1 (0, 0), 0);
    EXPECT_EQ (mat_xyz1 (2, cloud.width - 1), 3 * cloud.width - 1);   // = 29

    EXPECT_EQ (mat_xyz.cols (), cloud.width);
    EXPECT_EQ (mat_xyz.rows (), 3);
    EXPECT_EQ (mat_xyz (0, 0), 0);
    EXPECT_EQ (mat_xyz (2, cloud.width - 1), 3 * cloud.width - 1);    // = 29
  }

#ifdef NDEBUG
  if (Eigen::MatrixXf::Flags & Eigen::RowMajorBit)
  {
    Eigen::MatrixXf mat_yz = cloud.getMatrixXfMap (2, 4, 1);
    EXPECT_EQ (mat_yz.cols (), 2);
    EXPECT_EQ (mat_yz.rows (), cloud.width);
    EXPECT_EQ (mat_yz (0, 0), 1);
    EXPECT_EQ (mat_yz (cloud.width - 1, 1), 3 * cloud.width - 1);
    std::uint32_t j = 1;
    for (std::uint32_t i = 1; i < cloud.width*cloud.height; i+=4, j+=3)
    {
      Eigen::MatrixXf mat_yz = cloud.getMatrixXfMap (2, 4, i);
      EXPECT_EQ (mat_yz.cols (), 2);
      EXPECT_EQ (mat_yz.rows (), cloud.width);
      EXPECT_EQ (mat_yz (0, 0), j);
    }
  }
  else
  {
    Eigen::MatrixXf mat_yz = cloud.getMatrixXfMap (2, 4, 1);
    EXPECT_EQ (mat_yz.cols (), cloud.width);
    EXPECT_EQ (mat_yz.rows (), 2);
    EXPECT_EQ (mat_yz (0, 0), 1);
    EXPECT_EQ (mat_yz (1, cloud.width - 1), 3 * cloud.width - 1);
    std::uint32_t j = 1;
    for (std::uint32_t i = 1; i < cloud.width*cloud.height; i+=4, j+=3)
    {
      Eigen::MatrixXf mat_yz = cloud.getMatrixXfMap (2, 4, i);
      EXPECT_EQ (mat_yz.cols (), cloud.width);
      EXPECT_EQ (mat_yz.rows (), 2);
      EXPECT_EQ (mat_yz (0, 0), j);
    }
  }
#endif
}

TEST_F (pointCloudTest, pointCloud_clear)
{
  cloud.clear ();
  EXPECT_EQ (cloud.width, 0);
  EXPECT_EQ (cloud.height, 0);
}

TEST_F (pointCloudTest, pointCloud_insert_1)
{
  cloud.insert (cloud.end (), PointXYZ (1, 1, 1));
  EXPECT_FALSE (cloud.isOrganized ());
  EXPECT_EQ (cloud.width, 1);
}

TEST_F (pointCloudTest, pointCloud_insert_2)
{
  cloud.insert (cloud.end (), 5, PointXYZ (1, 1, 1));
  EXPECT_FALSE (cloud.isOrganized ());
  EXPECT_EQ (cloud.width, 5);
}

TEST_F (pointCloudTest, pointCloud_erase_1)
{
  cloud.insert (cloud.end (), 5, PointXYZ (1, 1, 1));
  cloud.erase (cloud.end () - 1);
  EXPECT_FALSE (cloud.isOrganized ());
  EXPECT_EQ (cloud.width, 4);

}

TEST_F (pointCloudTest, pointCloud_erase_2)
{
  cloud.insert (cloud.end (), 5, PointXYZ (1, 1, 1));
  cloud.erase (cloud.begin (), cloud.end ());
  EXPECT_FALSE (cloud.isOrganized ());
  EXPECT_EQ (cloud.width, 0);
}

TEST_F (pointCloudTest, pointCloud_emplace)
{
  cloud.emplace (cloud.end (), 1, 1, 1);
  EXPECT_FALSE (cloud.isOrganized ());
  EXPECT_EQ (cloud.width, 1);
}

TEST_F (pointCloudTest, pointCloud_emplace_back)
{
  auto& new_point = cloud.emplace_back (1, 1, 1);
  EXPECT_FALSE (cloud.isOrganized ());
  EXPECT_EQ (cloud.width, 1);
  EXPECT_EQ (&new_point, &cloud.back ());
}

TEST_F (pointCloudTest, pointCloud_resize_1)
{
  cloud.resize (640*360);
  EXPECT_FALSE (cloud.isOrganized ());
  EXPECT_EQ (cloud.width, 640*360);
}

TEST_F (pointCloudTest, pointCloud_resize_2)
{
  cloud.resize (640, 480);
  EXPECT_TRUE (cloud.isOrganized ());
  EXPECT_EQ (cloud.width, 640);
  EXPECT_EQ (cloud.height, 480);
}

TEST_F (pointCloudTest, pointCloud_resize_3)
{
  cloud.resize (640*360, PointXYZ (1, 1, 1));
  EXPECT_FALSE (cloud.isOrganized ());
  EXPECT_EQ (cloud.width, 640*360);
}

TEST_F (pointCloudTest, pointCloud_resize_4)
{
  cloud.resize (640, 480, PointXYZ (1, 1, 1));
  EXPECT_TRUE (cloud.isOrganized ());
  EXPECT_EQ (cloud.width, 640);
}

TEST_F (pointCloudTest, pointCloud_assign_1)
{
  cloud.assign (640*360, PointXYZ (1, 1, 1));
  EXPECT_FALSE (cloud.isOrganized ());
  EXPECT_EQ (cloud.width, 640*360);
}

TEST_F (pointCloudTest, pointCloud_assign_2)
{
  cloud.assign(640, 480, PointXYZ (1, 1, 1));
  EXPECT_TRUE (cloud.isOrganized ());
  EXPECT_EQ (cloud.width, 640);
}

TEST_F (pointCloudTest, pointCloud_assign_3)
{
  std::vector<PointXYZ> pointVec;
  pointVec.resize (640*360, PointXYZ (2, 3, 4));
  cloud.assign (pointVec.begin(), pointVec.end());
  EXPECT_FALSE (cloud.isOrganized ());
  EXPECT_EQ (cloud.width, 640*360);
}

TEST_F (pointCloudTest, pointCloud_assign_4)
{
  std::vector<PointXYZ> pointVec;
  pointVec.resize (640*360, PointXYZ (2, 3, 4));
  cloud.assign (pointVec.begin(), pointVec.end(), 640);
  EXPECT_TRUE (cloud.isOrganized ());
  EXPECT_EQ (cloud.width, 640);
}

TEST_F (pointCloudTest, pointCloud_assign_5)
{
  std::vector<PointXYZ> pointVec;
  pointVec.resize (640*480, PointXYZ (7, 7, 7));
  cloud.assign (pointVec.begin(), pointVec.end(), 460);
  EXPECT_FALSE (cloud.isOrganized ());
  EXPECT_EQ (cloud.width, 640*480);
}

TEST_F (pointCloudTest, pointCloud_assign_initializer_list_1)
{
  cloud.assign ({PointXYZ (3, 4, 5), PointXYZ (3, 4, 5), PointXYZ (3, 4, 5)});
  EXPECT_FALSE (cloud.isOrganized ());
  EXPECT_EQ (cloud.width, 3);
}

TEST_F (pointCloudTest, pointCloud_assign_initializer_list_2)
{
  cloud.assign ({PointXYZ (3, 4, 5), PointXYZ (3, 4, 5), PointXYZ (3, 4, 5), PointXYZ (3, 4, 5)}, 2);
  EXPECT_TRUE (cloud.isOrganized ());
  EXPECT_EQ (cloud.width, 2);
}

TEST_F (pointCloudTest, pointCloud_assign_initializer_list_3)
{
  cloud.assign ({PointXYZ (3, 4, 5), PointXYZ (3, 4, 5), PointXYZ (3, 4, 5)}, 6);
  EXPECT_FALSE (cloud.isOrganized ());
  EXPECT_EQ (cloud.width, 3);
}

TEST_F (pointCloudTest, pointCloud_pushback_1)
{
  cloud.push_back (PointXYZ (3, 4, 5));
  EXPECT_FALSE (cloud.isOrganized ());
  EXPECT_EQ (cloud.width, 1);
}

TEST_F (pointCloudTest, pointCloud_pushback_2)
{
  set_cloud(80*80, PointXYZ (1, 1, 1), 80);
  EXPECT_TRUE (cloud.isOrganized ());
  cloud.push_back (PointXYZ (3, 4, 5));
  EXPECT_EQ (cloud.width, (80*80) + 1);
}

TEST_F (pointCloudTest, pointCloud_transient_pushback)
{
  set_cloud (640*480, PointXYZ (1, 1, 1), 640);
  cloud.transient_push_back (PointXYZ(2, 2, 2));
  EXPECT_TRUE (cloud.isOrganized ());
  EXPECT_EQ (cloud.width, 640);
  EXPECT_EQ (cloud.size(), (640*480) + 1);
}

TEST_F (pointCloudTest, pointCloud_transient_emplaceback)
{
  set_cloud (640*480, PointXYZ (1, 1, 1), 640);
  auto& new_pointXYZ = cloud.transient_emplace_back (3, 3, 3);
  EXPECT_TRUE (cloud.isOrganized ());
  EXPECT_EQ (cloud.width, 640);
  EXPECT_EQ (cloud.size(), (640*480) + 1);
  EXPECT_EQ (&new_pointXYZ, &cloud.back ());
}

TEST_F (pointCloudTest, pointCloud_transient_insert_1)
{
  set_cloud (640*480, PointXYZ (1, 1, 1), 640);
  cloud.transient_insert (cloud.end (), PointXYZ (1, 1, 1));
  EXPECT_TRUE (cloud.isOrganized ());
  EXPECT_EQ (cloud.size(), (640*480) + 1);
  EXPECT_EQ (cloud.width, 640);
}

TEST_F (pointCloudTest, pointCloud_transient_insert_2)
{
  set_cloud (640*480, PointXYZ (1, 1, 1), 640);
  cloud.transient_insert (cloud.end (), 10, PointXYZ (1, 1, 1));
  EXPECT_TRUE (cloud.isOrganized ());
  EXPECT_EQ (cloud.size(), (640*480) + 10);
  EXPECT_EQ (cloud.width, 640);
}

TEST_F (pointCloudTest, pointCloud_transient_emplace)
{
  set_cloud (640*480, PointXYZ (1, 1, 1), 640);
  cloud.transient_emplace (cloud.end (), 4, 4, 4);
  EXPECT_TRUE (cloud.isOrganized ());
  EXPECT_EQ (cloud.width, 640);
  EXPECT_EQ (cloud.size(), (640*480) + 1);
}

TEST_F (pointCloudTest, pointCloud_transient_erase_1)
{
  set_cloud (640*480, PointXYZ (1, 1, 1), 640);
  cloud.transient_erase (cloud.end () - 1);
  EXPECT_TRUE (cloud.isOrganized ());
  EXPECT_EQ (cloud.width, 640);
  EXPECT_EQ (cloud.size(), (640*480) - 1);
}

TEST_F (pointCloudTest, pointCloud_transient_erase_2)
{
  set_cloud (640*480, PointXYZ (1, 1, 1), 640);
  cloud.transient_erase (cloud.begin (), cloud.end ());
  EXPECT_TRUE (cloud.isOrganized ());
  EXPECT_EQ (cloud.width, 640);
  EXPECT_EQ (cloud.size(), 0);
}

TEST_F (pointCloudTest, pointCloud_unorganized_concatenate_1)
{
  set_cloud (640*480, PointXYZ (1, 1, 1), 640);
  PointCloud<PointXYZ> new_unorganized_cloud;
  PointCloud<PointXYZ>::concatenate (new_unorganized_cloud, cloud);
  EXPECT_FALSE (new_unorganized_cloud.isOrganized ());
  EXPECT_EQ (new_unorganized_cloud.width, 640*480);
}

TEST_F (pointCloudTest, pointCloud_unorganized_concatenate_2)
{
  set_cloud (640*480, PointXYZ (1, 1, 1), 640);
  PointCloud<PointXYZ> new_unorganized_cloud;
  PointCloud<PointXYZ>::concatenate (new_unorganized_cloud, cloud);
  PointCloud<PointXYZ> unorganized_cloud_out;
  PointCloud<PointXYZ>::concatenate (new_unorganized_cloud, cloud, unorganized_cloud_out);
  EXPECT_FALSE (unorganized_cloud_out.isOrganized ());
  EXPECT_EQ (unorganized_cloud_out.width, 640*480*2);
}

TEST_F (pointCloudTest, pointCloud_unorganized_concatenate_3)
{
  set_cloud (640*480, PointXYZ (1, 1, 1), 640);
  PointCloud<PointXYZ> unorganized_cloud;
  PointCloud<PointXYZ>::concatenate (unorganized_cloud, cloud);
  PointCloud<PointXYZ> unorganized_cloud_out = cloud + unorganized_cloud;
  EXPECT_FALSE (unorganized_cloud_out.isOrganized ());
  EXPECT_EQ (unorganized_cloud_out.width, 640*480*2);
}

TEST_F (pointCloudTest, pointCloud_unorganized_concatenate_4)
{
  set_cloud (640*480, PointXYZ (1, 1, 1), 640);
  PointCloud<PointXYZ> unorganized_cloud;
  unorganized_cloud += cloud;
  EXPECT_FALSE (unorganized_cloud.isOrganized ());
  EXPECT_EQ (unorganized_cloud.width, 640*480);
}

TEST_F (pointCloudTest, pointCloud_at_with_throw)
{
  set_cloud (640*480, PointXYZ (1, 1, 1), 640);
  PointCloud<PointXYZ> unorganized_cloud;
  unorganized_cloud += cloud;
  EXPECT_THROW({unorganized_cloud.at (5, 5);}, UnorganizedPointCloudException);
}

TEST_F (pointCloudTest, pointCloud_at_no_throw)
{
  set_cloud (640*480, PointXYZ (1, 1, 1), 640);
  const auto& point_at = cloud.at (cloud.width - 1, cloud.height - 1);
  EXPECT_EQ(&point_at, &cloud.back());
}

TEST_F (pointCloudTest, pointCloud_organized_concatenate)
{
  cloud.resize (640, 480, PointXYZ (1, 1, 1));
  PointCloud<PointXYZ> organized_cloud1 = cloud;
  PointCloud<PointXYZ> organized_cloud2 = cloud;
  EXPECT_TRUE (organized_cloud1.isOrganized ());
  EXPECT_TRUE (organized_cloud2.isOrganized ());
  PointCloud<PointXYZ> organized_cloud_out = organized_cloud1 + organized_cloud2;
  EXPECT_FALSE (organized_cloud_out.isOrganized ());
  EXPECT_EQ (organized_cloud_out.width, 614400);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST (PCL, PointTypes)
{
  EXPECT_EQ (sizeof (PointXYZ), 16);
  EXPECT_EQ (__alignof (PointXYZ), 16);
  EXPECT_EQ (sizeof (PointXYZI), 32);
  EXPECT_EQ (__alignof (PointXYZI), 16);
  EXPECT_EQ (sizeof (PointXYZRGB), 32);
  EXPECT_EQ (__alignof (PointXYZRGB), 16);
  EXPECT_EQ (sizeof (PointXYZRGBA), 32);
  EXPECT_EQ (__alignof (PointXYZRGBA), 16);
  EXPECT_EQ (sizeof (Normal), 32);
  EXPECT_EQ (__alignof (Normal), 16);
  EXPECT_EQ (sizeof (PointNormal), 48);
  EXPECT_EQ (__alignof (PointNormal), 16);
  EXPECT_EQ (sizeof (PointXYZRGBNormal), 48);
  EXPECT_EQ (__alignof (PointXYZRGBNormal), 16);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T> class XYZPointTypesTest : public ::testing::Test { };
using XYZPointTypes = ::testing::Types<BOOST_PP_SEQ_ENUM(PCL_XYZ_POINT_TYPES)>;
TYPED_TEST_SUITE(XYZPointTypesTest, XYZPointTypes);
TYPED_TEST(XYZPointTypesTest, GetVectorXfMap)
{
  TypeParam pt;
  for (std::size_t i = 0; i < 3; ++i)
    EXPECT_EQ (&pt.data[i], &pt.getVector3fMap () (i));
  for (std::size_t i = 0; i < 4; ++i)
    EXPECT_EQ (&pt.data[i], &pt.getVector4fMap () (i));
}

TYPED_TEST(XYZPointTypesTest, GetArrayXfMap)
{
  TypeParam pt;
  for (std::size_t i = 0; i < 3; ++i)
    EXPECT_EQ (&pt.data[i], &pt.getArray3fMap () (i));
  for (std::size_t i = 0; i < 4; ++i)
    EXPECT_EQ (&pt.data[i], &pt.getArray4fMap () (i));
}

template <typename T> class NormalPointTypesTest : public ::testing::Test { };
using NormalPointTypes = ::testing::Types<BOOST_PP_SEQ_ENUM(PCL_NORMAL_POINT_TYPES)>;
TYPED_TEST_SUITE(NormalPointTypesTest, NormalPointTypes);
TYPED_TEST(NormalPointTypesTest, GetNormalVectorXfMap)
{
  TypeParam pt;
  for (std::size_t i = 0; i < 3; ++i)
    EXPECT_EQ (&pt.data_n[i], &pt.getNormalVector3fMap () (i));
  for (std::size_t i = 0; i < 4; ++i)
    EXPECT_EQ (&pt.data_n[i], &pt.getNormalVector4fMap () (i));
}

template <typename T> class RGBPointTypesTest : public ::testing::Test { };
using RGBPointTypes = ::testing::Types<BOOST_PP_SEQ_ENUM(PCL_RGB_POINT_TYPES)>;
TYPED_TEST_SUITE(RGBPointTypesTest, RGBPointTypes);
TYPED_TEST(RGBPointTypesTest, GetRGBVectorXi)
{
  TypeParam pt; pt.r = 1; pt.g = 2; pt.b = 3; pt.a = 4;
  EXPECT_EQ (pt.r, pt.getRGBVector3i () (0));
  EXPECT_EQ (pt.g, pt.getRGBVector3i () (1));
  EXPECT_EQ (pt.b, pt.getRGBVector3i () (2));
  EXPECT_EQ (pt.r, pt.getRGBVector4i () (0));
  EXPECT_EQ (pt.g, pt.getRGBVector4i () (1));
  EXPECT_EQ (pt.b, pt.getRGBVector4i () (2));
  EXPECT_EQ (pt.a, pt.getRGBVector4i () (3));
  EXPECT_EQ (pt.r, pt.getRGBAVector4i () (0));
  EXPECT_EQ (pt.g, pt.getRGBAVector4i () (1));
  EXPECT_EQ (pt.b, pt.getRGBAVector4i () (2));
  EXPECT_EQ (pt.a, pt.getRGBAVector4i () (3));
}

TYPED_TEST(RGBPointTypesTest, GetBGRVectorXcMap)
{
  TypeParam pt;
  EXPECT_EQ (&pt.b, &pt.getBGRVector3cMap () (0));
  EXPECT_EQ (&pt.g, &pt.getBGRVector3cMap () (1));
  EXPECT_EQ (&pt.r, &pt.getBGRVector3cMap () (2));
  EXPECT_EQ (&pt.b, &pt.getBGRAVector4cMap () (0));
  EXPECT_EQ (&pt.g, &pt.getBGRAVector4cMap () (1));
  EXPECT_EQ (&pt.r, &pt.getBGRAVector4cMap () (2));
  EXPECT_EQ (&pt.a, &pt.getBGRAVector4cMap () (3));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST (PCL, Intersections)
{
  Eigen::VectorXf zline (6), yline (6);
  zline[0] = 0.543892f; zline[1] = -0.515623f; zline[2] = 1.321f;   zline[3] = 0.0266191f; zline[4] = 0.600215f;  zline[5] = -0.0387667f;
  yline[0] = 0.493479f; yline[1] = 0.169246f;  yline[2] = 1.22677f; yline[3] = 0.5992f;    yline[4] = 0.0505085f; yline[5] = 0.405749f;

  Eigen::Vector4f pt;
  EXPECT_TRUE (pcl::lineWithLineIntersection (zline, yline, pt));
  EXPECT_NEAR (pt[0], 0.574544, 1e-3);
  EXPECT_NEAR (pt[1], 0.175526, 1e-3);
  EXPECT_NEAR (pt[2], 1.27636,  1e-3);
  EXPECT_EQ (pt[3], 0);

  zline << 0.545203f, -0.514419f, 1.31967f, 0.0243372f, 0.597946f, -0.0413579f;
  yline << 0.492706f,  0.164196f, 1.23192f, 0.598704f,  0.0442014f, 0.411328f;
  EXPECT_FALSE (pcl::lineWithLineIntersection (zline, yline, pt));
  //intersection: [ 3.06416e+08    15.2237     3.06416e+08       4.04468e-34 ]
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST (PCL, CopyIfFieldExists)
{
  PointXYZRGBNormal p;

  p.x = 1.0; p.y = 2;  p.z = 3.0;
  p.r = 127; p.g = 64; p.b = 254;
  p.normal_x = 1.0; p.normal_y = 0.0; p.normal_z = 0.0;

  using FieldList = pcl::traits::fieldList<PointXYZRGBNormal>::type;
  bool is_x = false, is_y = false, is_z = false, is_rgb = false,
       is_normal_x = false, is_normal_y = false, is_normal_z = false;

  float x_val, y_val, z_val, normal_x_val, normal_y_val, normal_z_val, rgb_val;
  x_val = y_val = z_val = std::numeric_limits<float>::quiet_NaN ();
  normal_x_val = normal_y_val = normal_z_val = std::numeric_limits<float>::quiet_NaN ();
  rgb_val = std::numeric_limits<float>::quiet_NaN ();

  pcl::for_each_type<FieldList> (CopyIfFieldExists<PointXYZRGBNormal, float> (p, "x", is_x, x_val));
  EXPECT_TRUE (is_x);
  EXPECT_EQ (x_val, 1.0);
  pcl::for_each_type<FieldList> (CopyIfFieldExists<PointXYZRGBNormal, float> (p, "y", is_y, y_val));
  EXPECT_TRUE (is_y);
  EXPECT_EQ (y_val, 2.0);
  pcl::for_each_type<FieldList> (CopyIfFieldExists<PointXYZRGBNormal, float> (p, "z", is_z, z_val));
  EXPECT_TRUE (is_z);
  EXPECT_EQ (z_val, 3.0);
  pcl::for_each_type<FieldList> (CopyIfFieldExists<PointXYZRGBNormal, float> (p, "rgb", is_rgb, rgb_val));
  EXPECT_TRUE (is_rgb);
  std::uint32_t rgb;
  std::memcpy (&rgb, &rgb_val, sizeof(rgb_val));
  EXPECT_EQ (rgb, 0xff7f40fe);      // alpha is 255
  pcl::for_each_type<FieldList> (CopyIfFieldExists<PointXYZRGBNormal, float> (p, "normal_x", is_normal_x, normal_x_val));
  EXPECT_TRUE (is_normal_x);
  EXPECT_EQ (normal_x_val, 1.0);
  pcl::for_each_type<FieldList> (CopyIfFieldExists<PointXYZRGBNormal, float> (p, "normal_y", is_normal_y, normal_y_val));
  EXPECT_TRUE (is_normal_y);
  EXPECT_EQ (normal_y_val, 0.0);
  pcl::for_each_type<FieldList> (CopyIfFieldExists<PointXYZRGBNormal, float> (p, "normal_z", is_normal_z, normal_z_val));
  EXPECT_TRUE (is_normal_z);
  EXPECT_EQ (normal_z_val, 0.0);

  pcl::for_each_type<FieldList> (CopyIfFieldExists<PointXYZRGBNormal, float> (p, "x", x_val));
  EXPECT_EQ (x_val, 1.0);

  float xx_val = -1.0;
  pcl::for_each_type<FieldList> (CopyIfFieldExists<PointXYZRGBNormal, float> (p, "xx", xx_val));
  EXPECT_EQ (xx_val, -1.0);
  bool is_xx = true;
  pcl::for_each_type<FieldList> (CopyIfFieldExists<PointXYZRGBNormal, float> (p, "xx", is_xx, xx_val));
  EXPECT_FALSE (is_xx);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST (PCL, SetIfFieldExists)
{
  PointXYZRGBNormal p;

  p.x = p.y = p.z = 0.0;
  p.r = p.g = p.b = 0;
  p.normal_x = p.normal_y = p.normal_z = 0.0;

  using FieldList = pcl::traits::fieldList<PointXYZRGBNormal>::type;
  pcl::for_each_type<FieldList> (SetIfFieldExists<PointXYZRGBNormal, float> (p, "x", 1.0));
  EXPECT_EQ (p.x, 1.0);
  pcl::for_each_type<FieldList> (SetIfFieldExists<PointXYZRGBNormal, float> (p, "y", 2.0));
  EXPECT_EQ (p.y, 2.0);
  pcl::for_each_type<FieldList> (SetIfFieldExists<PointXYZRGBNormal, float> (p, "z", 3.0));
  EXPECT_EQ (p.z, 3.0);
  pcl::for_each_type<FieldList> (SetIfFieldExists<PointXYZRGBNormal, float> (p, "normal_x", 1.0));
  EXPECT_EQ (p.normal_x, 1.0);
  pcl::for_each_type<FieldList> (SetIfFieldExists<PointXYZRGBNormal, float> (p, "normal_y", 0.0));
  EXPECT_EQ (p.normal_y, 0.0);
  pcl::for_each_type<FieldList> (SetIfFieldExists<PointXYZRGBNormal, float> (p, "normal_z", 0.0));
  EXPECT_EQ (p.normal_z, 0.0);

//  pcl::PointXY p1;
//  pcl::for_each_type<pcl::traits::fieldList<pcl::PointXY>::type> (pcl::SetIfFieldExists<pcl::PointXY, float> (p1, "intensity", 3.0));
//
//  pcl::PFHSignature125 p2;
//  pcl::for_each_type<pcl::traits::fieldList<pcl::PFHSignature125>::type> (pcl::SetIfFieldExists<pcl::PFHSignature125, float*> (p2, "intensity", 3.0));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST (PCL, IsSamePointType)
{
  bool status = isSamePointType<PointXYZ, PointXYZ> ();
  EXPECT_TRUE (status);
  status = isSamePointType<PointXYZ, PointXY> ();
  EXPECT_FALSE (status);
  status = isSamePointType<PointXY, PointXYZ> ();
  EXPECT_FALSE (status);
  status = isSamePointType<PointNormal, PointNormal> ();
  EXPECT_TRUE (status);
  status = isSamePointType<PointNormal, PointXYZRGBNormal> ();
  EXPECT_FALSE (status);
  status = isSamePointType<PointXYZRGB, PointXYZRGB> ();
  EXPECT_TRUE (status);

  // Even though it's the "same" type, rgb != rgba
  status = isSamePointType<PointXYZRGB, PointXYZRGBA> ();
  EXPECT_FALSE (status);
  status = isSamePointType<PointXYZRGBA, PointXYZRGB> ();
  EXPECT_FALSE (status);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST (PCL, HasField)
{
  // has_field
  EXPECT_TRUE ((pcl::traits::has_field<pcl::Normal, pcl::fields::curvature>::value));
  EXPECT_FALSE ((pcl::traits::has_field<pcl::PointXYZ, pcl::fields::curvature>::value));
  // has_all_fields
  EXPECT_TRUE ((pcl::traits::has_all_fields<pcl::PointXYZRGB, boost::mpl::vector<pcl::fields::x, pcl::fields::rgb> >::value));
  EXPECT_FALSE ((pcl::traits::has_all_fields<pcl::PointXYZ, boost::mpl::vector<pcl::fields::x, pcl::fields::rgb> >::value));
  // has_any_field
  EXPECT_TRUE ((pcl::traits::has_any_field<pcl::PointXYZ, boost::mpl::vector<pcl::fields::x, pcl::fields::normal_x> >::value));
  EXPECT_TRUE ((pcl::traits::has_any_field<pcl::Normal, boost::mpl::vector<pcl::fields::x, pcl::fields::normal_x> >::value));
  EXPECT_FALSE ((pcl::traits::has_any_field<pcl::RGB, boost::mpl::vector<pcl::fields::x, pcl::fields::normal_x> >::value));
  // has_xyz
  EXPECT_TRUE ((pcl::traits::has_xyz<pcl::PointXYZ>::value));
  EXPECT_FALSE ((pcl::traits::has_xyz<pcl::Normal>::value));
  // has_normal
  EXPECT_TRUE ((pcl::traits::has_normal<pcl::PointNormal>::value));
  EXPECT_FALSE ((pcl::traits::has_normal<pcl::PointXYZ>::value));
  // has_curvature
  EXPECT_TRUE ((pcl::traits::has_curvature<pcl::PointNormal>::value));
  EXPECT_FALSE ((pcl::traits::has_curvature<pcl::RGB>::value));
  // has_intensity
  EXPECT_TRUE ((pcl::traits::has_intensity<pcl::PointXYZI>::value));
  EXPECT_FALSE ((pcl::traits::has_intensity<pcl::PointXYZ>::value));
  // has_color
  EXPECT_TRUE ((pcl::traits::has_color<pcl::PointXYZRGB>::value));
  EXPECT_TRUE ((pcl::traits::has_color<pcl::PointXYZRGBA>::value));
  EXPECT_FALSE ((pcl::traits::has_color<pcl::PointXYZ>::value));
  // has_label
  EXPECT_TRUE ((pcl::traits::has_label<pcl::PointXYZL>::value));
  EXPECT_FALSE ((pcl::traits::has_label<pcl::Normal>::value));
}

TEST (PCL, GetMinMax3D)
{
  pcl::PointCloud<pcl::PointXYZ> cloud;
  cloud.emplace_back ( 0.0f,      0.0f,  0.0f);
  cloud.emplace_back (10.0f, -10000.0f,  1.0f);
  cloud.emplace_back ( 5.0f,      5.0f,  0.0f);
  cloud.emplace_back (-5.0f,      0.0f, -0.5f);

  pcl::PointXYZ min_pt, max_pt;
  Eigen::Vector4f min_vec, max_vec;

  pcl::getMinMax3D (cloud, min_pt, max_pt);
  EXPECT_EQ (min_pt.x, -5.0f);
  EXPECT_EQ (min_pt.y, -10000.0f);
  EXPECT_EQ (min_pt.z, -0.5f);
  EXPECT_EQ (max_pt.x, 10.0f);
  EXPECT_EQ (max_pt.y, 5.0f);
  EXPECT_EQ (max_pt.z, 1.0f);

  pcl::getMinMax3D (cloud, min_vec, max_vec);
  EXPECT_EQ (min_vec.x (), -5.0f);
  EXPECT_EQ (min_vec.y (), -10000.0f);
  EXPECT_EQ (min_vec.z (), -0.5f);
  EXPECT_EQ (max_vec.x (), 10.0f);
  EXPECT_EQ (max_vec.y (), 5.0f);
  EXPECT_EQ (max_vec.z (), 1.0f);

  pcl::PointIndices pindices;
  pindices.indices.push_back (0);
  pindices.indices.push_back (2);
  pcl::getMinMax3D (cloud, pindices, min_vec, max_vec);
  EXPECT_EQ (min_vec.x (), 0.0f);
  EXPECT_EQ (min_vec.y (), 0.0f);
  EXPECT_EQ (min_vec.z (), 0.0f);
  EXPECT_EQ (max_vec.x (), 5.0f);
  EXPECT_EQ (max_vec.y (), 5.0f);
  EXPECT_EQ (max_vec.z (), 0.0f);

  pcl::Indices indices;
  indices.push_back (1);
  indices.push_back (3);
  pcl::getMinMax3D (cloud, indices, min_vec, max_vec);
  EXPECT_EQ (min_vec.x (), -5.0f);
  EXPECT_EQ (min_vec.y (), -10000.0f);
  EXPECT_EQ (min_vec.z (), -0.5f);
  EXPECT_EQ (max_vec.x (), 10.0f);
  EXPECT_EQ (max_vec.y (), 0.0f);
  EXPECT_EQ (max_vec.z (), 1.0f);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST (PCL, GetMaxDistance)
{
  PointCloud<PointXYZ> cloud;
  Eigen::Vector4f max_pt, max_exp_pt;
  const Eigen::Vector4f pivot_pt (Eigen::Vector4f::Zero ());

  // populate cloud
  cloud.resize (3);
  cloud[0].data[0] = 4.f; cloud[0].data[1] = 3.f;
  cloud[0].data[2] = 0.f; cloud[0].data[3] = 0.f;
  cloud[1].data[0] = 0.f; cloud[1].data[1] = 0.f;
  cloud[1].data[2] = 0.f; cloud[1].data[3] = 1000.f;  //This term should not influence max dist
  cloud[2].data[0] = -1.5f; cloud[2].data[1] = 1.5f;
  cloud[2].data[2] = -.5f; cloud[2].data[3] = 0.f;

  // No indices specified
  max_exp_pt = cloud[0].getVector4fMap ();
  getMaxDistance (cloud, pivot_pt, max_pt);
  test::EXPECT_EQ_VECTORS (max_exp_pt, max_pt);

  // Specifying indices
  Indices idx (2);
  idx[0] = 1; idx[1] = 2;
  max_exp_pt = cloud[2].getVector4fMap ();
  getMaxDistance (cloud, idx, pivot_pt, max_pt);
  test::EXPECT_EQ_VECTORS (max_exp_pt, max_pt);
}

/* ---[ */
int
main (int argc, char** argv)
{
  testing::InitGoogleTest (&argc, argv);
  return (RUN_ALL_TESTS ());
}
/* ]--- */
