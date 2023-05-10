#ifndef JOB_H
#define JOB_H


/**
* Boundary condition:
*
* 1 - movement in the x axis direction is prohibited
* 2 - movement in the direction of the y axis is prohibited
* 3 - it is forbidden to move in both x and y directions
*/
class Job
{
private:
    int nodesCount;


public:
    Job() {}
    void job_writeToFile(const char* inFile, const char* outFile);
    int getNodesCount() const;
};





#endif // JOB_H
