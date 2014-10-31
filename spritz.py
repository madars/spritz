#!/usr/bin/env python
from __future__ import division
import math
import random

def calculate_spritz(N, num_tries, num_outputs):
    assert num_outputs >= N * N * N * 5

    expected = num_outputs/(N**3)
    expected_chi = (N-1)**3
    expected_stddev_chi = math.sqrt(2*expected_chi)

    chis = []

    for trial in xrange(num_tries):
        count_table = [None] * N
        for i in xrange(N):
            count_table[i] = [None] * N
            for j in xrange(N):
                count_table[i][j] = [0] * N

        S = range(N)
        random.SystemRandom().shuffle(S)

        i, j, k = 0, 0, 0
        z2, z1, z = 0, 0, 0

        for outpos in xrange(num_outputs):
            i = (i + 1) % N
            j = (k + S[(j + S[i]) % N]) % N
            k = (i + k + S[j]) % N
            S[i], S[j] = S[j], S[i]
            new_z = S[(j + S[(i + S[(z + k) % N]) % N]) % N]

            z2, z1, z = z1, z, new_z

            count_table[i][z2][z] += 1

        chi = 0
        for i in xrange(N):
            for j in xrange(N):
                for k in xrange(N):
                    chi += (count_table[i][j][k] - expected)**2
        chi /= expected
        chis.append(chi)

        print "trial %d of %d, chi = %0.3f (%0.3f %+0.3f * %0.3f)" % (trial, num_tries, chi, expected_chi, (chi-expected_chi)/expected_stddev_chi, expected_stddev_chi)

    chi_avg = sum(chi for chi in chis) / num_tries
    chi_stddev = math.sqrt(sum((chi - chi_avg)**2 / num_tries for chi in chis))

    print "Average chi squared: %0.3f" % chi_avg
    print "stddev(chi squared): %0.3f" % chi_stddev
    print "expected chi: %0.3f" % expected_chi
    print "expected standard deviation: %0.3f" % expected_stddev_chi
    print "average is x standard deviations away from average for x = %0.3f" % abs((chi_avg-expected_chi)/expected_stddev_chi)

calculate_spritz(16, 2**18, 2**18)
