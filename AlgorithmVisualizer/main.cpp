#include <iostream>

#include <QApplication>

#include "main_window.h"
#include "algorithm_visualizer_quad_tree.h"
/*
 * What kind of algorithms can be visualized?
 *      - 2D stuff as KD-tree or Quad tree
 *      - Sorting algs
 *      - Tree and graph related algorithms
 *      - Pathfinding algs
 * What these algs have in common when it comes to visualization?
 *      - They can be represented via simple shapes
 *      - They need some gui elements as buttons and input fields
 *      - They need to work with mouse input
 * What are common to most algs GUI elements?
 *      - Reset button DONE
 *      - Clear button DONE
 *      - Stop/Resume button DONE
 * What functions are important for all visualisations?
 *      - Mouse events
 *      - Resize event
 * What function main window needs?
 *      - Ability to select algorithms via dropdown DONE
 * Is there need for general input side panel?
 *      - Different algs will need different UI so it would be handy
 *      to be able to generate 5 buttons and 3 input filds automatically
 *      - In any case for each alg that needs custom UI it will be necessary
 *      to deal with things like layouts
 *      - So it makes sence to create abstraction for UI holder
 *      to avoid messing with layouts each time
 * Things to implement:
 *      - Main window with:
 *          - Reset, Clear, Stop/Resume buttons DONE
 *          - Alg selection dropdown DONE
 *          - Mechanism to forward Reset, Clear, Stop/Resume signals DONE
 *      - Base class for all alg visualizers with:
 *          - API for Reset, Clear, Stop/Resume DONE
 *      - Base class for UI side panel with:
 *          - API for creation of:
 *              - Horizontal combo box out of n provided widgets DONE
 *          - FlowLayout FAIL (replaced with H and B boxes) DONE
 *          - Transparent background FAIL (no need cuz everything is transparent by default) DONE
 */


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    AlgorithmVisualizerDummy anotherDummy;
    AlgorithmVisualizerQuadTree quadTreeAV;
    w.addAlgorithmVisualizer(&anotherDummy);
    w.addAlgorithmVisualizer(&quadTreeAV);
    w.show();

    return a.exec();
}
