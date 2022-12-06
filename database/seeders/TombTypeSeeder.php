<?php

namespace Database\Seeders;

use Illuminate\Database\Seeder;
use Illuminate\Support\Facades\DB;

class TombTypeSeeder extends Seeder
{
    /**
     * Run the database seeds.
     *
     * @return void
     */
    public function run()
    {
        $tomb_types = [
            ['name' => ' - ', 'thickness' => 0],
            ['name' => 'Cassettone da 3', 'thickness' => 3],
            ['name' => 'Cassettone 2+2', 'thickness' => 4],
            ['name' => 'Base e tetto da 3', 'thickness' => 3],
            ['name' => 'Base e tetto 2+2', 'thickness' => 4],
            ['name' => 'Tappo', 'thickness' => 0]
        ];

        DB::table('tomb_types')->insert($tomb_types);
    }
}
