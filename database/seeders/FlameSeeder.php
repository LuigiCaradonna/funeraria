<?php

namespace Database\Seeders;

use Illuminate\Database\Seeder;
use Illuminate\Support\Facades\DB;

class FlameSeeder extends Seeder
{
    /**
     * Run the database seeds.
     *
     * @return void
     */
    public function run()
    {
        $flames = [
            ['name' => ' - '],
            ['name' => 'Classica'],
            ['name' => 'Rosa semifrontale'],
            ['name' => 'Rosa semifrontale iride']
        ];

        DB::table('flames')->insert($flames);
    }
}
