<?php

namespace Database\Seeders;

use Illuminate\Database\Seeder;
use Illuminate\Support\Facades\DB;

class CrossSeeder extends Seeder
{
    /**
     * Run the database seeds.
     *
     * @return void
     */
    public function run()
    {
        $crosses = [
            ['name' => ' - '],
            ['name' => 'KS220'],
            ['name' => 'KS240'],
            ['name' => 'KS220 Mod'],
        ];

        DB::table('crosses')->insert($crosses);
    }
}
