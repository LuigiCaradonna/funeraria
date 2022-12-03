<?php

namespace Database\Seeders;

use Illuminate\Database\Seeder;
use Illuminate\Support\Facades\DB;

class OutlineSeeder extends Seeder
{
    /**
     * Run the database seeds.
     *
     * @return void
     */
    public function run()
    {
        $outlines = [
            ['name' => 'No Ikon'],
            ['name' => 'Stondato no Ikon'],
            ['name' => 'Stondato Ikon'],
            ['name' => 'KS301'],
            ['name' => 'KS311'],
            ['name' => 'KS320'],
            ['name' => 'KS380'],
            ['name' => 'KS390'],
        ];

        DB::table('outlines')->insert($outlines);
    }
}
