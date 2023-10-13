#include <iostream>
#include <random>
#include <vector>
#include <memory>
#include <algorithm>
#include <chrono>

float sq(const float a)
{
    return a * a;
}

struct Point {
  float x, y;

  float distance(const Point& p) const
    {
        return sq(p.x - x) + sq(p.y - y);
    }

    const Point& findClosestPoint(std::vector<Point> &pts) const
    {
        if(pts.size() == 0)
        {
            throw "Haaaa";
        }

        float closestDistance = distance(pts[0]);
        Point& closestPoint = pts[0];

        for(auto&& p: pts)
        {
            const float d = distance(p);

            if(d < closestDistance)
            {
                closestPoint = p;
                closestDistance = d;
            }
        }

        return closestPoint;
    }
};


struct Tree
{
    Tree(std::unique_ptr<Tree> _left, std::unique_ptr<Tree> _right, Point _p, bool _splitOnX)
    :left(std::move(_left))
     ,right(std::move(_right))
     ,p(_p)
     ,splitOnX(_splitOnX)
    {}

    static std::unique_ptr<Tree> makeTreeInternal(std::vector<Point>::iterator start, std::vector<Point>::iterator end, bool sortOnX)
    {
        if(start == end)
        {
            return nullptr;
        }

        // std::distance on std::vector iterators is O(1)
        auto middle = std::distance(start, end) / 2;

        if(sortOnX)
        {
           std::nth_element(start, start + middle, end, [](const Point &p0, const Point &p1)
                   {
                         return p0.x < p1.x;
                   });
        }
        else
        {
           std::nth_element(start, start + middle, end, [](const Point &p0, const Point &p1)
                   {
                        return p0.y < p1.y;
                   });
        }

        auto leftTree = makeTreeInternal(start, start + middle, !sortOnX);
        auto rightTree = makeTreeInternal(start + middle + 1, end, !sortOnX);

        return std::make_unique<Tree>(std::move(leftTree), std::move(rightTree), *(start + middle), sortOnX);
    }

    const Point findClosestPoint(const Point &pLookup, Point currentClosestPoint, float currentSmallestDistance) const
    {
        float distance = p.distance(pLookup);
        if(distance < currentSmallestDistance)
        {
            currentSmallestDistance = distance;
            currentClosestPoint = p;
        }

        if(splitOnX)
        {
            if(pLookup.x < p.x)
            {
                if(left)
                   return left->findClosestPoint(pLookup, currentClosestPoint, currentSmallestDistance);
            }
            else
            {
                if(right)
                  return right->findClosestPoint(pLookup, currentClosestPoint, currentSmallestDistance);
            }
        }
        else
        {
            if(pLookup.y < p.y)
            {
                if(left)
                   return left->findClosestPoint(pLookup, currentClosestPoint, currentSmallestDistance);
            }
            else
            {
                if(right)
                   return right->findClosestPoint(pLookup, currentClosestPoint, currentSmallestDistance);
            }
        }

        return currentClosestPoint;
    }

    const Point findClosestPoint(const Point &pLookup) const
    {
        return findClosestPoint(pLookup, p, p.distance(pLookup)); 
    }


    static std::unique_ptr<Tree> makeTree(std::vector<Point> pts)
    {
        return makeTreeInternal(pts.begin(), pts.end(), true);
    }

    std::unique_ptr<Tree> left, right;
    bool splitOnX;
    Point p;
};

template<typename T>
auto to_ms(T v)
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(v);
}

int main() {
  std::cout << "Test\n";

  std::default_random_engine generator;

  std::uniform_real_distribution<float> distribution(0, 1);

  const int N = 10000000;

  auto t0 = std::chrono::system_clock::now();
  std::vector<Point> v;
  v.reserve(N);

  for (int i = 0; i < N; i++) {
    v.push_back(Point{distribution(generator), distribution(generator)});
  }

  auto t1 = std::chrono::system_clock::now();
     
  auto tree = Tree::makeTree(v);

  auto t2 = std::chrono::system_clock::now();

  const Point p{0.5, 0.5};

  auto t3 = std::chrono::system_clock::now();

  for(int i = 0; i < 10; i++)
  {
    const auto closest = p.findClosestPoint(v);
    std::cout << closest.x << " " << closest.y << "\n";
    std::cout << closest.distance(p) << "\n";
  }

  auto t4 = std::chrono::system_clock::now();

  for(int i = 0; i < 1000; i++)
  {
     const auto closest = tree->findClosestPoint(p);
     std::cout << closest.x << " " << closest.y << "\n";
     std::cout << closest.distance(p) << "\n";
  }

  auto t5 = std::chrono::system_clock::now();

  std::cout << "Allocate points: " << to_ms(t1 - t0) << "\n";
  std::cout << "Build tree: " << to_ms(t2 - t1) << "\n";
  std::cout << "Query linear: " << to_ms((t4 - t3) * 100) << "\n";
  std::cout << "Query tree: " << to_ms(t5 - t4) << "\n";
  return 0;
}

// Benchmark:
// 10 millions points
// - Creation des points: 1.100s
//
// - O(n) pour 1000 queries: 9.726
//
// - Creation de l'arbre: 12s
// - 1000 queries: 12s
