#!/usr/bin/perl

use List::Util qw(sum);

for ($n = 1; $n < 9; $n++) {
    @values = ();
    for ($i = 0; $i < 20; $i++)  {
	push @values, `../build/src/tests/multithreading/pdevs-multithreading-main $n`;
    }
    $sum = sum(@values);
    $average = $sum / 20;

    $sum2 = 0;
    for ($i = 0; $i < 20; $i++)  {
	$e = $values[$i] - $average;
	$sum2 = $sum2 + $e * $e;
    }

    print $n . "\t" . $average . "\t" . (sqrt($sum2) / 20) . "\n";
}
