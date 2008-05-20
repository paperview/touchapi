#ifndef __TRACKING
#define __TRACKING

#include <list>
#include <map>

class BlobTracker
{
public: 

	BlobTracker()
	{
		IDCounter = 0;
	}

	//assigns IDs to each blob in the contourFinder
	void track(ofxCvContourFinder* newBlobs)
	{
		//initialize ID's of all blobs
		for(int i=0; i<newBlobs->nBlobs; i++)
			newBlobs->blobs[i].id=-1;

		//go through all tracked blobs to compute nearest new point
		for(int i=0; i<trackedBlobs.size(); i++)
		{
			int winner = trackKnn(newBlobs, &(trackedBlobs[i]), 5, 0);

			if(winner==-1) //track has died, mark it for deletion
				trackedBlobs[i].id = -1; //delete
			else //still alive, have to update
			{
				//if winning new blob was labeled winner by another track\
				  then compare with this track to see which is closer
				if(newBlobs->blobs[winner].id!=-1)
				{
					//find the currently assigned blob
					int j;
					for(j=0; j<trackedBlobs.size(); j++)
					{
						if(trackedBlobs[j].id==newBlobs->blobs[winner].id)
							break;
					}

					if(j==trackedBlobs.size())//got to end without finding it
					{
						newBlobs->blobs[winner].id = trackedBlobs[i].id;
						trackedBlobs[i] = newBlobs->blobs[winner];				
					}
					else //found it, compare with current blob
					{
						double x = newBlobs->blobs[winner].centroid.x;
						double y = newBlobs->blobs[winner].centroid.y;
						double xOld = trackedBlobs[j].centroid.x;
						double yOld = trackedBlobs[j].centroid.y;
						double xNew = trackedBlobs[i].centroid.x;
						double yNew = trackedBlobs[i].centroid.y;
						double distOld = (x-xOld)*(x-xOld)+(y-yOld)*(y-yOld);
						double distNew = (x-xNew)*(x-xNew)+(y-yNew)*(y-yNew);

						//if this track is closer, update the ID of the blob\
						  and update the track,\
						  otherwise delete this track.. it's dead
						if(distNew<distOld) //update
						{
							newBlobs->blobs[winner].id = trackedBlobs[i].id;
							trackedBlobs[i] = newBlobs->blobs[winner];
						}
						else //delete
							trackedBlobs[i].id = -1;
					}
				}
				else //no conflicts, so simply update
				{
					newBlobs->blobs[winner].id = trackedBlobs[i].id;
					trackedBlobs[i] = newBlobs->blobs[winner];		
				}
			}
		}

		//remove every tracked labeled as dead (ID='-1')
		for(int i=0; i<trackedBlobs.size(); i++)
		{
			if(trackedBlobs[i].id==-1)
			{
				trackedBlobs.erase(trackedBlobs.begin()+i,
								   trackedBlobs.begin()+i+1);
				i--; //decrement one since we removed an element
			}
		}

		//now every new blob should be either labeled with a tracked ID or\
		  have ID of -1... if the ID is -1... we need to make a new track
		for(int i=0; i<newBlobs->nBlobs; i++)
		{
			if(newBlobs->blobs[i].id==-1)
			{
				newBlobs->blobs[i].id=IDCounter++;
				trackedBlobs.push_back(newBlobs->blobs[i]);
			}
		}

	}

private:
	//Finds the blob in 'newBlobs' that is closest to the trackedBlob with index\
	  'ind' according to the KNN algorithm and returns the index of the winner
	//newBlobs	= list of blobs detected in the latest frame
	//track		= current tracked blob being tested
	//k			= number of nearest neighbors to consider\
				  1,3,or 5 are common numbers..\
				  must always be an odd number to avoid tying
	//thresh	= threshold for optimization
	int trackKnn(ofxCvContourFinder *newBlobs, ofxCvBlob *track,
				 int k, double thresh = 0)
	{
		int winner = -1; //initially label track as '-1'=dead
		if((k%2)==0) k++; //if k is not an odd number, add 1 to it

		//if it exists, square the threshold to use as square distance
		if(thresh>0)
			thresh *= thresh;

		//list of neighbor point index and respective distances
		std::list<std::pair<int,double>> nbors;
		std::list<std::pair<int,double>>::iterator iter;

		//find 'k' closest neighbors of testpoint
		double x, y, xT, yT, dist;
		for(int i=0; i<newBlobs->nBlobs; i++)
		{
			x = newBlobs->blobs[i].centroid.x;
			y = newBlobs->blobs[i].centroid.y;
			xT = track->centroid.x;
			yT = track->centroid.y;
			dist = (x-xT)*(x-xT)+(y-yT)*(y-yT);

			if(dist<=thresh)//it's good, apply label if no label yet and return
			{
				winner = i;
				return winner;
			}

			//===
			//check if this blob is closer to the point than what we've seen\
			  so far and add it to the index/distance list if positive

			//search the list for the first point with a longer distance
			for(iter=nbors.begin(); iter!=nbors.end()
				&& dist>=iter->second; iter++);

			if((iter!=nbors.end())||(nbors.size()<k)) //it's valid, insert it
			{
				nbors.insert(iter, 1, std::pair<int, double>(i, dist));
				//too many items in list, get rid of farthest neighbor
				if(nbors.size()>k)
					nbors.pop_back();
			}
			//===
		}

		//we now have k nearest neighbors who cast a vote, and the majority\
		  wins. we use each class average distance to the target to break any\
		  possible ties.

		// a mapping from labels (IDs) to count/distance
		std::map<int, std::pair<int, double>> votes;

		//remember:
		//iter->first = index of newBlob
		//iter->second = distance of newBlob to current tracked blob
		for(iter=nbors.begin(); iter!=nbors.end(); iter++)
		{
			//add up how many counts each neighbor got
			int count = ++(votes[iter->first].first);
			double dist = (votes[iter->first].second+=iter->second);

			/* check for a possible tie and break with distance */
			if(count>votes[winner].first || count==votes[winner].first
				&& dist<votes[winner].second) 
			{
				winner = iter->first;
			}
		}

		return winner;
	}

	std::vector<ofxCvBlob>	trackedBlobs; //tracked blobs
	int						IDCounter;	  //counter of last blob
};

#endif