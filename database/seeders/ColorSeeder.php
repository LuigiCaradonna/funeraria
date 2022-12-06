<?php

namespace Database\Seeders;

use Illuminate\Database\Seeder;
use Illuminate\Support\Facades\DB;

class ColorSeeder extends Seeder
{
    /**
     * Run the database seeds.
     *
     * @return void
     */
    public function run()
    {
        $colors = [
            ['name' => ' - '],
            ['name' => 'Argento'],
            ['name' => 'Oro'],
            ['name' => 'Oro bronzo'],
            ['name' => 'Bronzo chiaro'],
            ['name' => 'Bronzo scuro'],
            ['name' => 'Nero'],
            ['name' => 'Bianco'],
            ['name' => 'Panna']
        ];

        DB::table('colors')->insert($colors);
    }
}
