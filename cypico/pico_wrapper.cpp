#include "pico_wrapper.h"

int pico_cluster_objects(float* rcsq, int n_detections)
{
    n_detections = cluster_detections(&rcsq[0], n_detections);
    return n_detections;
}

int pico_detect_objects(const unsigned char* image, 
                        const int height,
                        const int width, 
                        const int width_step,
                        const unsigned char* cascades, 
                        const int max_detections,
                        const int n_orientations, 
                        const float* orientations,
                        const float scale_factor, 
                        const float stride_factor,
                        const float min_size, 
                        const float q_cutoff,
                        float* rcsq,
                        float* os)
{
    int n_detections = 0;
    int n_new_detections = 0;
    int i = 0, k = 0;
    // Scan the image at n_orientations different orientations
    for(i = 0; i < n_orientations; i++) {
        
        n_new_detections = find_objects(&rcsq[4*n_detections],           //results
                                        max_detections - n_detections, //max-detections
                                        (void *)cascades,              //cascade
                                        orientations[i],               //angle (0 - 0.0, 1 - 2*pi)
                                        (void *)image,                 //pixels
                                        height,                        //nrows
                                        width,                         //ncols
                                        width_step,                    //ldim
                                        scale_factor,                  //scalefactor
                                        stride_factor,                 //stridefactor
                                        min_size,                      //minsize
                                        MIN(height, width));            //maxsize
       
       for (k = 0; k < n_new_detections; k++) {
           os[n_detections + k] = orientations[i];
       }
       
       n_detections += n_new_detections;
    }
    return n_detections;
}
