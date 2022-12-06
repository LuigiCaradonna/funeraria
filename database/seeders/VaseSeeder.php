<?php

namespace Database\Seeders;

use Illuminate\Database\Seeder;
use Illuminate\Support\Facades\DB;

class VaseSeeder extends Seeder
{
    /**
     * Run the database seeds.
     *
     * @return void
     */
    public function run()
    {
        $vases = [
            ['name' => ' - '],
            ['name' => 'Calice'],
            ['name' => 'Vaschetta'],
            ['name' => 'Mezza tazza'],
        ];

        DB::table('vases')->insert($vases);
    }
}
