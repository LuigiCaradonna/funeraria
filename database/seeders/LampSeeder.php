<?php

namespace Database\Seeders;

use Illuminate\Database\Seeder;
use Illuminate\Support\Facades\DB;

class LampSeeder extends Seeder
{
    /**
     * Run the database seeds.
     *
     * @return void
     */
    public function run()
    {
        $lamps = [
            ['name' => ' - '],
            ['name' => 'Lumino'],
            ['name' => 'Anello'],
            ['name' => 'Cubetto'],
        ];

        DB::table('lamps')->insert($lamps);
    }
}
