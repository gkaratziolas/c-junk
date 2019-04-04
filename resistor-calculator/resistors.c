#include <stdio.h>
#include <stdlib.h>

double e12[] = {10, 12, 15, 18, 22, 27, 33, 39, 47, 56, 68, 82};
double e24[] = {10, 11, 12, 13, 15, 16, 18, 20, 22, 24, 27, 30, 33, 36, 39, 43, 47, 51, 56, 62, 68, 75, 82, 91};
double e48[] = {10.0, 10.5, 11.0, 11.5, 12.1, 12.7, 13.3, 14.0, 14.7, 15.4, 16.2, 16.9, 17.8, 18.7, 19.6, 20.5, 21.5, 22.6, 23.7, 24.9, 26.1, 27.4, 28.7, 30.1, 31.6, 33.2, 34.8, 36.5, 38.3, 40.2, 42.2, 44.2, 46.4, 48.7, 51.1, 53.6, 56.2, 59.0, 61.9, 64.9, 68.1, 71.5, 75.0, 78.7, 82.5, 86.6, 90.9, 95.3};
double e96[] = {10.0, 10.2, 10.5, 10.7, 11.0, 11.3, 11.5, 11.8, 12.1, 12.4, 12.7, 13.0, 13.3, 13.7, 14.0, 14.3, 14.7, 15.0, 15.4, 15.8, 16.2, 16.5, 16.9, 17.4, 17.8, 18.2, 18.7, 19.1, 19.6, 20.0, 20.5, 21.0, 21.5, 22.1, 22.6, 23.2, 23.7, 24.3, 24.9, 25.5, 26.1, 26.7, 27.4, 28.0, 28.7, 29.4, 30.1, 30.9, 31.6, 32.4, 33.2, 34.0, 34.8, 35.7, 36.5, 37.4, 38.3, 39.2, 40.2, 41.2, 42.2, 43.2, 44.2, 45.3, 46.4, 47.5, 48.7, 49.9, 51.1, 52.3, 53.6, 54.9, 56.2, 57.6, 59.0, 60.4, 61.9, 63.4, 64.9, 66.5, 68.1, 69.8, 71.5, 73.2, 75.0, 76.8, 78.7, 80.6, 82.5, 84.5, 86.6, 88.7, 90.9, 93.1, 95.3, 97.6};

void nearest_ratio(double target, double *series, int series_length);
double nearest_resistor(double target, double *series, int series_length);
void print_resistor(double value);
double dabs(double a);
double series(double r1, double r2);
double parallel(double r1, double r2);
void print_resistor(double value);

int main(int argc, char **argv)
{
        nearest_ratio(atof(argv[1]), e96, 96);
        //double x = nearest_resistor(atof(argv[1]), e96, 96);
        //print_resistor(x);
        return 0;
}

double dabs(double a)
{
        if (a < 0)
                return -a;
        return a;
}

void nearest_ratio(double target, double *series, int series_length)
{
        double original_target = target;

        int inverse = 0;
        if (target < 1) {
                inverse = 1;
                target = 1 / target;
        }

        double scale = 1;
        while (target > 1) {
                target = target / 10;
                scale  = scale * 10;
        }

        double min_dist = 1;
        double dist;
        int nearest_i, nearest_j, i, j;

        for (i=0; i<series_length; i++) {
                for (j=i; j<series_length; j++) {
                        dist = dabs(target - series[i]/series[j]);
                        if (dist < min_dist) {
                                min_dist = dist;
                                nearest_i = i;
                                nearest_j = j;
                        }
                }
        }

        double R1, R2, ratio, error;
        if (inverse) {
                R1 = series[nearest_j];
                R2 = scale * series[nearest_i];
        } else {
                R1 = scale * series[nearest_i];
                R2 = series[nearest_j];
        }

        ratio = R1/R2;
        error = (original_target - ratio) / original_target;

        printf("Nearest resistor ratio:\n");
        printf("R1 = ");
        print_resistor(R1);
        printf("R2 = ");
        print_resistor(R2);
        printf("Ratio = %f\n", R1/R2);
        printf("Error = %.2f%%\n", 100*error );

}

double nearest_resistor(double target, double *series, int series_length)
{
        // Ensure target is in decade between 10 and 100
        double scale = 1;
        while (target >= 100) {
                target = target / 10;
                scale = scale * 10;
        }
        while (target < 10) {
                target = target * 10;
                scale = scale / 10;
        }

        printf("target: %f\n", target);

        // Find the first series value larger than the target
        int i;
        for (i=1; i<series_length; i++){
                if (target - series[i] < 0)
                        break;
        }

        // If the target is bigger than anything in the series, return the 
        // series max
        if (i == series_length) {
                return series[i] * scale;
        }

        if (series[i]-target < target-series[i]) {
                return series[i] * scale;
        }

        return series[i-1] * scale;
}

double series(double r1, double r2)
{
        return r1 + r2;
}

double parallel(double r1, double r2)
{
        return (r1*r2)/(r1+r2);
}

void print_resistor(double value)
{
        char unit = ' ';
        if (value < 0.001) {
                value = value * 1000000;
                unit = 'u';
        } else if (value < 1) {
                value = value * 1000;
                unit = 'm';
        } else if (value < 1000) {
                unit = 'R';
        } else if (value < 1000000) {
                value = value / 1000;
                unit = 'k';
        } else if (value < 1000000000) {
                value = value / 1000000;
                unit = 'M';
        } else if (value < 1000000000000) {
                value = value / 1000000000;
                unit = 'G';
        }

        printf("%.3g %c\n", value, unit);
}
