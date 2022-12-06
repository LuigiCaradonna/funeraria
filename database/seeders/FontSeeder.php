<?php

namespace Database\Seeders;

use Illuminate\Database\Seeder;
use Illuminate\Support\Facades\DB;

class FontSeeder extends Seeder
{
    /**
     * Run the database seeds.
     *
     * @return void
     */
    public function run()
    {
        $fonts = [
            ['name' => ' - '],
            ['name' => 'Times New Roman'],
            ['name' => 'English 111 Presto'],
            ['name' => 'English 111 Vivace'],
            ['name' => 'English 111 Adagio'],
            ['name' => 'Murray Hill'],
            ['name' => 'Incimar corsivo']
        ];

        DB::table('fonts')->insert($fonts);
    }
}
