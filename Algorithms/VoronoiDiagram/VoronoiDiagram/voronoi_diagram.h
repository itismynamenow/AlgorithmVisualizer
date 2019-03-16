#ifndef VORONOI_DIAGRAM_H
#define VORONOI_DIAGRAM_H
#include <unordered_map>
#include <QVector2D>
#include "delaunator.h"

class MyPolygon{
public:
    MyPolygon(std::initializer_list<double> coords){
        for(unsigned i=0;i<coords.size();i+=2){
            auto x = *(coords.begin()+i);
            auto y = *(coords.begin()+i+1);
            vertices.push_back(QVector2D(x,y));
        }
    }
    MyPolygon(const std::vector<QVector2D> &vertices): vertices(vertices){}
    void setCoords(std::initializer_list<double> coords){
        vertices.clear();
        for(unsigned i=0;i<coords.size();i+=2){
            auto x = *(coords.begin()+i);
            auto y = *(coords.begin()+i+1);
            vertices.push_back(QVector2D(x,y));
        }
    }
    std::vector<QVector2D> vertices;
};

struct QVector2DHasher {
    std::size_t operator()(const QVector2D &vec) const{
        std::size_t h = 0;
        h ^= std::hash<double>{}(vec.x())  + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<double>{}(vec.y())  + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

class VoronoiDiagram{

public:
    using MAP_VVV = std::unordered_map<QVector2D,std::vector<QVector2D>,QVector2DHasher>;
    void setPoints(const std::vector<QVector2D> &points){
        if(points.size()>=3){
            this->clear();
            this->points = points;
            performTriangulation(this->points);
            mapPointsToTriangles();
            for(const auto &point: points){
                auto polygonVertices = findPolygonVertices(point);
                polygonsVertices.insert(polygonsVertices.end(),polygonVertices.begin(),polygonVertices.end());
                auto polygon = makePolygon(point);
                if(polygon.vertices.size()>=3){
                    polygons.push_back(polygon);
                }
            }
        }
    }
    void performTriangulation(const std::vector<QVector2D> &points){
        auto doubles = pointsToDoubles(points);
        delaunator::Delaunator d(doubles);
        triangles.clear();
        for(std::size_t i = 0; i < d.triangles.size(); i+=3) {
            MyPolygon triangle{
                d.coords[2 * d.triangles[i]],        //tx0
                d.coords[2 * d.triangles[i] + 1],    //ty0
                d.coords[2 * d.triangles[i + 1]],    //tx1
                d.coords[2 * d.triangles[i + 1] + 1],//ty1
                d.coords[2 * d.triangles[i + 2]],    //tx2
                d.coords[2 * d.triangles[i + 2] + 1] //ty2
            };
            triangles.push_back(std::move(triangle));
        }
    }
    void mapPointsToTriangles(){
        for(auto &triangle: triangles){
            for(const auto &vertex: triangle.vertices){
                if(pointsToTriangles.count(vertex)>0){
                    pointsToTriangles.at(vertex).push_back(&triangle);
                }else{
                    pointsToTriangles.insert({{vertex, std::vector<MyPolygon*>{&triangle}}});
                }
            }
        }
    }
    MyPolygon makePolygon(const QVector2D &point){
        std::vector<QVector2D> polygonVertices;
        auto pointTriangles = pointsToTriangles.at(point);
        MAP_VVV vertexToAdjecentVertices = mapAdjecentVertices(point, pointTriangles);
        auto orderedVertices = getOrderedVertices(vertexToAdjecentVertices);
        if(vertexToAdjecentVertices.at(orderedVertices.at(0)).size()==2){
            for(unsigned i=0;i<orderedVertices.size();i++){
                auto midPoint = (point+orderedVertices.at(i))/2;
                auto direction = point - orderedVertices.at(i);
                auto perpendicular = QVector2D(direction.y(),-direction.x());

                unsigned iNext = (i+1)%orderedVertices.size();
                auto nextMidPoint = (point+orderedVertices.at(iNext))/2;
                auto nextDirection = point - orderedVertices.at(iNext);
                auto nextPerpendicular = QVector2D(nextDirection.y(),-nextDirection.x());

                auto intersection = getLineIntersection(midPoint,midPoint+perpendicular,nextMidPoint,nextMidPoint+nextPerpendicular);
                polygonVertices.push_back(intersection);
            }
        }
        return MyPolygon(polygonVertices);
    }
    std::vector<QVector2D> findPolygonVertices(const QVector2D &point){
        std::vector<QVector2D> polygonVertices;
        auto pointTriangles = pointsToTriangles.at(point);
        MAP_VVV vertexToAdjecentVertices = mapAdjecentVertices(point, pointTriangles);
        auto orderedVertices = getOrderedVertices(vertexToAdjecentVertices);
        for(unsigned i=0;i<orderedVertices.size()-1;i++){
            auto midPoint = (point+orderedVertices.at(i))/2;
            auto direction = point - orderedVertices.at(i);
            auto perpendicular = QVector2D(direction.y(),-direction.x());

            auto nextMidPoint = (point+orderedVertices.at(i+1))/2;
            auto nextDirection = point - orderedVertices.at(i+1);
            auto nextPerpendicular = QVector2D(nextDirection.y(),-nextDirection.x());

            auto intersection = getLineIntersection(midPoint,midPoint+perpendicular,nextMidPoint,nextMidPoint+nextPerpendicular);
            polygonVertices.push_back(intersection);
        }
        return polygonVertices;
    }
    static QVector2D getLineIntersection(QVector2D A, QVector2D B, QVector2D C, QVector2D D)
    {
        // Line AB represented as a1x + b1y = c1
        double a1 = B.y() - A.y();
        double b1 = A.x() - B.x();
        double c1 = a1*(A.x()) + b1*(A.y());

        // Line CD represented as a2x + b2y = c2
        double a2 = D.y() - C.y();
        double b2 = C.x() - D.x();
        double c2 = a2*(C.x())+ b2*(C.y());

        double determinant = a1*b2 - a2*b1;

        if (determinant == 0)
        {
            // The lines are parallel. This is simplified
            // by returning a pair of FLT_MAX
            return QVector2D(99999999,99999999);
        }
        else
        {
            double x = (b2*c1 - b1*c2)/determinant;
            double y = (a1*c2 - a2*c1)/determinant;
            return QVector2D(x, y);
        }
    }
    std::vector<QVector2D> getOrderedVertices(const MAP_VVV &vertexToAdjecentVertices){
        QVector2D firstVertex;
        bool isOpenPolygon = false;
        for(const auto &pair: vertexToAdjecentVertices){
            if(pair.second.size()==1){
                isOpenPolygon = true;
                firstVertex = pair.first;
                break;
            }
        }
        if(!isOpenPolygon) firstVertex = vertexToAdjecentVertices.begin()->first;
        std::vector<QVector2D> orderedVertices;
        QVector2D currVertex = firstVertex;
        QVector2D prevVertex;
        do{
            QVector2D nextVertex;
            orderedVertices.push_back(currVertex);
            if(currVertex == firstVertex){
                nextVertex = vertexToAdjecentVertices.at(currVertex).at(0);
            }else if(vertexToAdjecentVertices.at(currVertex).size() == 1){
                break;
            }
            else{
                if(prevVertex == vertexToAdjecentVertices.at(currVertex).at(0)) nextVertex = vertexToAdjecentVertices.at(currVertex).at(1);
                else nextVertex = vertexToAdjecentVertices.at(currVertex).at(0);
            }
            prevVertex = currVertex;
            currVertex = nextVertex;
        }while(currVertex != firstVertex);
        return orderedVertices;
    }
    MAP_VVV mapAdjecentVertices(const QVector2D &point, const std::vector<MyPolygon*> &pointTriangles){
        MAP_VVV vertexToAdjecentVertices;
        for(const auto &pointTriangle: pointTriangles){
            std::vector<QVector2D> vertices;
            for(auto &vertex: pointTriangle->vertices){
                if(vertex != point) vertices.push_back(vertex);
            }
            for(int i=0;i<2;i++){
                auto vertex0 = vertices.at(i%2);
                auto vertex1 = vertices.at((i+1)%2);
                if(vertexToAdjecentVertices.count(vertex0)>0) vertexToAdjecentVertices.at(vertex0).push_back(vertex1);
                else vertexToAdjecentVertices.insert({vertex0,std::vector<QVector2D>{vertex1}});
            }
        }
        return vertexToAdjecentVertices;
    }
    std::vector<double> pointsToDoubles(const std::vector<QVector2D> &points) const{
        std::vector<double> doubles;
        for(const auto &point: points){
            doubles.push_back(point.x());
            doubles.push_back(point.y());
        }
        return doubles;
    }
    void clear(){
        points.clear();
        triangles.clear();
        pointsToTriangles.clear();
        polygonsVertices.clear();
        polygons.clear();
    }
    std::vector<QVector2D> points;
    std::vector<MyPolygon> triangles;
    std::vector<MyPolygon> polygons;
    std::vector<QVector2D> polygonsVertices;
    std::unordered_map<QVector2D,std::vector<MyPolygon*>,QVector2DHasher> pointsToTriangles;
};

#endif // VORONOI_DIAGRAM_H
